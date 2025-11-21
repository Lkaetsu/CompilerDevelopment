import tkinter as tk
from tkinter import filedialog, messagebox
import threading
import subprocess
import queue
import os
import sys

class VMGUI:
    def __init__(self, master):
        self.master = master
        master.title("MVD Interface")

        self.proc = None
        self.stdout_queue = queue.Queue()
        self.running = False
        self.ready = False
        self.step_mode = False
        self.waiting_input = False
        self.auto_run = False

        # Top frame for file selection
        top = tk.Frame(master)
        top.pack(fill=tk.X, padx=8, pady=4)

        self.file_var = tk.StringVar()
        tk.Label(top, text="Arquivo .o:").pack(side=tk.LEFT)
        tk.Entry(top, textvariable=self.file_var, width=50).pack(side=tk.LEFT, padx=4)
        tk.Button(top, text="Procurar", command=self.select_file).pack(side=tk.LEFT)

        # Mode buttons
        mode_frame = tk.Frame(master)
        mode_frame.pack(fill=tk.X, padx=8, pady=4)
        tk.Button(mode_frame, text="Iniciar Direto", command=self.start_direct).pack(side=tk.LEFT)
        tk.Button(mode_frame, text="Iniciar Passo", command=self.start_step).pack(side=tk.LEFT, padx=4)
        self.step_btn = tk.Button(mode_frame, text="Step", command=self.do_step, state=tk.DISABLED)
        self.step_btn.pack(side=tk.LEFT, padx=4)
        self.auto_btn = tk.Button(mode_frame, text="Auto", command=self.toggle_auto_run, state=tk.DISABLED)
        self.auto_btn.pack(side=tk.LEFT, padx=4)
        self.abort_btn = tk.Button(mode_frame, text="Abortar", command=self.abort, state=tk.DISABLED)
        self.abort_btn.pack(side=tk.LEFT, padx=4)

        # Input for RD
        input_frame = tk.Frame(master)
        input_frame.pack(fill=tk.X, padx=8, pady=4)
        tk.Label(input_frame, text="Entrada RD:").pack(side=tk.LEFT)
        self.input_entry = tk.Entry(input_frame, width=15)
        self.input_entry.pack(side=tk.LEFT)
        self.send_input_btn = tk.Button(input_frame, text="Enviar", command=self.send_input, state=tk.DISABLED)
        self.send_input_btn.pack(side=tk.LEFT, padx=4)

        # Log area
        self.text = tk.Text(master, height=25, wrap="none")
        self.text.pack(fill=tk.BOTH, expand=True, padx=8, pady=4)
        self.text.configure(state=tk.DISABLED)

        # Scrollbars
        yscroll = tk.Scrollbar(self.text, orient=tk.VERTICAL, command=self.text.yview)
        self.text.configure(yscrollcommand=yscroll.set)
        yscroll.pack(side=tk.RIGHT, fill=tk.Y)

        master.protocol("WM_DELETE_WINDOW", self.on_close)
        self.master.after(100, self.poll_stdout)

    def log(self, line):
        self.text.configure(state=tk.NORMAL)
        self.text.insert(tk.END, line + "\n")
        self.text.see(tk.END)
        self.text.configure(state=tk.DISABLED)

    def select_file(self):
        path = filedialog.askopenfilename(title="Selecione arquivo objeto", filetypes=[("Objeto","*.o"), ("Todos","*.*")])
        if path:
            self.file_var.set(path)

    def start_process(self):
        if self.proc is not None:
            messagebox.showerror("Erro", "Processo já em execução.")
            return False
        file_path = self.file_var.get().strip()
        if not file_path:
            messagebox.showerror("Erro", "Selecione o arquivo .o")
            return False
        if not os.path.exists(file_path):
            messagebox.showerror("Erro", "Arquivo não encontrado.")
            return False
        exe = self.find_executable()
        if exe is None:
            messagebox.showerror("Erro", "Executável mvd não encontrado. Compile primeiro.")
            return False
        cmd = [exe, file_path, "--iface"]
        try:
            self.proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1)
        except Exception as e:
            messagebox.showerror("Erro", f"Falha ao iniciar processo: {e}")
            self.proc = None
            return False
        self.running = True
        self.ready = False
        self.waiting_input = False
        threading.Thread(target=self.read_stdout, daemon=True).start()
        self.abort_btn.configure(state=tk.NORMAL)
        return True

    def find_executable(self):
        # procura por mvd_back.exe ou mvd_back (Linux) no diretório atual
        candidates = ["mvd_back.exe", "mvd_back"]
        for c in candidates:
            if os.path.exists(c):
                return c
        return None

    def start_direct(self):
        if not self.start_process():
            return
        self.step_mode = False
        self.step_btn.configure(state=tk.DISABLED)
        self.auto_btn.configure(state=tk.DISABLED)
        self.log("Modo direto: aguardando READY para enviar DIRECT.")

    def start_step(self):
        if not self.start_process():
            return
        self.step_mode = True
        self.step_btn.configure(state=tk.DISABLED)
        self.auto_btn.configure(state=tk.NORMAL)
        self.log("Modo passo: aguardando READY para habilitar Step.")

    def read_stdout(self):
        while self.running and self.proc and self.proc.stdout:
            line = self.proc.stdout.readline()
            if not line:
                break
            self.stdout_queue.put(line.rstrip("\n"))
        self.running = False

    def poll_stdout(self):
        try:
            while True:
                line = self.stdout_queue.get_nowait()
                self.handle_line(line)
        except queue.Empty:
            pass
        if self.running:
            self.master.after(100, self.poll_stdout)
        else:
            self.log("Processo finalizado.")
            self.step_btn.configure(state=tk.DISABLED)
            self.auto_btn.configure(state=tk.DISABLED)
            self.abort_btn.configure(state=tk.DISABLED)
            self.send_input_btn.configure(state=tk.DISABLED)

    def handle_line(self, line):
        if line.startswith("EVENT:"):
            self.parse_event(line)
        else:
            self.log(line)

    def parse_event(self, line):
        self.log(line)
        if line.startswith("EVENT:READY"):
            self.ready = True
            if self.step_mode:
                # habilita step e envia primeiro CONTINUE manual? usuário decide
                self.step_btn.configure(state=tk.NORMAL)
            else:
                # envia DIRECT automaticamente
                self.send_command("DIRECT")
        elif line.startswith("EVENT:INPUT_REQUEST"):
            self.waiting_input = True
            self.send_input_btn.configure(state=tk.NORMAL)
            self.input_entry.focus_set()
        elif line.startswith("EVENT:HALT") or line.startswith("EVENT:ABORTED"):
            self.running = False
        elif line.startswith("EVENT:DONE"):
            # após execução de uma instrução em modo passo, re-habilitar Step
            if self.step_mode and not self.auto_run and not self.waiting_input:
                self.step_btn.configure(state=tk.NORMAL)
        # STACK / OUTPUT / INSTR já são apenas logados

    def send_command(self, cmd):
        if self.proc and self.proc.stdin and self.running:
            try:
                self.proc.stdin.write(cmd + "\n")
                self.proc.stdin.flush()
            except Exception as e:
                self.log(f"Falha ao enviar comando: {e}")

    def do_step(self):
        if not self.ready or not self.running or self.waiting_input:
            return
        self.step_btn.configure(state=tk.DISABLED)
        self.send_command("CONTINUE")

    def toggle_auto_run(self):
        if not self.step_mode:
            return
        self.auto_run = not self.auto_run
        if self.auto_run:
            self.auto_btn.configure(text="Auto (ON)")
            self.step_btn.configure(state=tk.DISABLED)
            self.master.after(50, self.auto_step_loop)
        else:
            self.auto_btn.configure(text="Auto")
            if self.running and not self.waiting_input:
                self.step_btn.configure(state=tk.NORMAL)

    def auto_step_loop(self):
        if self.auto_run and self.running and not self.waiting_input:
            self.send_command("CONTINUE")
            self.master.after(50, self.auto_step_loop)

    def send_input(self):
        if not self.waiting_input:
            return
        val = self.input_entry.get().strip()
        if not val.isdigit() and not (val.startswith('-') and val[1:].isdigit()):
            messagebox.showerror("Erro", "Digite um inteiro válido.")
            return
        self.send_command(val)
        self.waiting_input = False
        self.send_input_btn.configure(state=tk.DISABLED)
        self.input_entry.delete(0, tk.END)
        # Se estava em auto-run retoma
        if self.auto_run:
            self.master.after(50, self.auto_step_loop)
        else:
            if self.step_mode:
                self.step_btn.configure(state=tk.NORMAL)

    def abort(self):
        if self.proc and self.running:
            self.send_command("ABORT")
            # força término se não responder
            self.master.after(500, self.force_kill)

    def force_kill(self):
        if self.running and self.proc:
            try:
                self.proc.kill()
            except Exception:
                pass
            self.running = False

    def on_close(self):
        if self.proc and self.running:
            if messagebox.askyesno("Sair", "Abortar execução e sair?"):
                self.abort()
                self.master.after(600, self.master.destroy)
            else:
                return
        else:
            self.master.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = VMGUI(root)
    root.mainloop()
