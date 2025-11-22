import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
import threading
import queue
import os
import sys

class MVDGui:
    def __init__(self, master):
        self.master = master
        master.title("MVD Virtual Machine")

        self.proc = None
        self.out_q = queue.Queue()
        self.running = False
        self.step_mode = tk.IntVar(value=1)
        self.rd_pending = False

        top = tk.Frame(master)
        top.pack(fill=tk.X, padx=8, pady=4)

        tk.Label(top, text="Arquivo .o:").pack(side=tk.LEFT)
        self.file_var = tk.StringVar()
        tk.Entry(top, textvariable=self.file_var, width=50).pack(side=tk.LEFT, padx=4)
        tk.Button(top, text="Procurar", command=self.select_file).pack(side=tk.LEFT)

        mode_frame = tk.Frame(master)
        mode_frame.pack(fill=tk.X, padx=8, pady=2)
        tk.Label(mode_frame, text="Modo:").pack(side=tk.LEFT)
        tk.Radiobutton(mode_frame, text="Direto", value=1, variable=self.step_mode).pack(side=tk.LEFT)
        tk.Radiobutton(mode_frame, text="Passo", value=2, variable=self.step_mode).pack(side=tk.LEFT)

        btn_frame = tk.Frame(master)
        btn_frame.pack(fill=tk.X, padx=8, pady=4)
        tk.Button(btn_frame, text="Compilar MVD", command=self.compile_vm).pack(side=tk.LEFT)
        tk.Button(btn_frame, text="Iniciar", command=self.start).pack(side=tk.LEFT)
        self.step_btn = tk.Button(btn_frame, text="Avançar", command=self.send_step, state=tk.DISABLED)
        self.step_btn.pack(side=tk.LEFT)
        self.abort_btn = tk.Button(btn_frame, text="Abortar", command=self.abort, state=tk.DISABLED)
        self.abort_btn.pack(side=tk.LEFT)

        rd_frame = tk.Frame(master)
        rd_frame.pack(fill=tk.X, padx=8, pady=4)
        tk.Label(rd_frame, text="Entrada RD:").pack(side=tk.LEFT)
        self.rd_var = tk.StringVar()
        tk.Entry(rd_frame, textvariable=self.rd_var, width=15).pack(side=tk.LEFT)
        self.rd_btn = tk.Button(rd_frame, text="Enviar", command=self.send_rd, state=tk.DISABLED)
        self.rd_btn.pack(side=tk.LEFT)

        self.log = tk.Text(master, height=30, wrap="none")
        self.log.pack(fill=tk.BOTH, expand=True, padx=8, pady=4)
        self.log.configure(state=tk.DISABLED)

        yscroll = tk.Scrollbar(self.log, orient=tk.VERTICAL, command=self.log.yview)
        self.log.configure(yscrollcommand=yscroll.set)
        yscroll.pack(side=tk.RIGHT, fill=tk.Y)

        master.protocol("WM_DELETE_WINDOW", self.on_close)

    def append(self, text):
        self.log.configure(state=tk.NORMAL)
        self.log.insert(tk.END, text + "\n")
        self.log.see(tk.END)
        self.log.configure(state=tk.DISABLED)

    def select_file(self):
        path = filedialog.askopenfilename(title="Selecione arquivo .o", filetypes=[("Objeto","*.o"),("Todos","*.*")])
        if path:
            self.file_var.set(path)

    def compile_vm(self):
        # compila mvd_back.c em mvd_back.exe (Windows) ou mvd_back (unix)
        src = "mvd_back.c"
        if not os.path.exists(src):
            messagebox.showerror("Erro", f"{src} não encontrado no diretório atual.")
            return
        exe_name = "mvd_back.exe" if os.name == "nt" else "mvd_back"
        cmd = ["gcc", src, "-o", exe_name]
        try:
            r = subprocess.run(cmd, capture_output=True, text=True)
            if r.returncode != 0:
                messagebox.showerror("Erro", f"Falha na compilação:\n{r.stderr}")
            else:
                self.append(f"VM compilada: {exe_name}")
        except Exception as e:
            messagebox.showerror("Erro", f"Execução do gcc falhou: {e}")

    def start(self):
        if self.running:
            messagebox.showwarning("Aviso", "Já em execução.")
            return
        file_path = self.file_var.get().strip()
        if not file_path:
            messagebox.showerror("Erro", "Selecione o arquivo .o")
            return
        if not os.path.exists(file_path):
            messagebox.showerror("Erro", "Arquivo .o não encontrado.")
            return
        exe = "mvd_back.exe" if os.name == "nt" else "mvd_back"
        if not os.path.exists(exe):
            messagebox.showerror("Erro", "Compile a VM primeiro (Compilar MVD).")
            return
        mode = str(self.step_mode.get())
        cmd = [exe, file_path, mode]
        try:
            self.proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1)
        except Exception as e:
            messagebox.showerror("Erro", f"Falha ao iniciar: {e}")
            self.proc = None
            return
        self.running = True
        self.abort_btn.configure(state=tk.NORMAL)
        if mode == '2':
            self.step_btn.configure(state=tk.NORMAL)
        threading.Thread(target=self.read_stdout, daemon=True).start()
        self.master.after(100, self.drain_queue)
        self.append("Processo iniciado.")

    def read_stdout(self):
        while self.running and self.proc and self.proc.stdout:
            line = self.proc.stdout.readline()
            if not line:
                break
            line = line.rstrip('\n')
            self.out_q.put(line)
        self.running = False

    def drain_queue(self):
        updated = False
        while True:
            try:
                line = self.out_q.get_nowait()
            except queue.Empty:
                break
            self.append(line)
            updated = True
            # detectar RD
            if 'ENTRADA DO PROGRAMA:' in line:
                self.rd_pending = True
                self.rd_btn.configure(state=tk.NORMAL)
        if not self.running and self.proc:
            self.finish()
        else:
            self.master.after(100, self.drain_queue)

    def send_step(self):
        if not self.proc or not self.running:
            return
        try:
            self.proc.stdin.write('\n')
            self.proc.stdin.flush()
        except Exception:
            pass

    def send_rd(self):
        if not self.rd_pending:
            return
        val = self.rd_var.get().strip()
        if not val.isdigit() and not (val.startswith('-') and val[1:].isdigit()):
            messagebox.showerror("Erro", "Digite um inteiro válido para RD.")
            return
        try:
            self.proc.stdin.write(val + '\n')
            self.proc.stdin.flush()
            self.rd_pending = False
            self.rd_btn.configure(state=tk.DISABLED)
            self.rd_var.set('')
        except Exception:
            pass

    def abort(self):
        if self.proc and self.running:
            try:
                self.proc.stdin.write('q\n')
                self.proc.stdin.flush()
            except Exception:
                pass
            try:
                self.proc.kill()
            except Exception:
                pass
        self.finish()
        self.append("Abortado pelo usuário.")

    def finish(self):
        self.running = False
        if self.proc:
            self.proc.stdout.close()
            self.proc.stdin.close()
            self.proc = None
        self.step_btn.configure(state=tk.DISABLED)
        self.abort_btn.configure(state=tk.DISABLED)
        self.rd_btn.configure(state=tk.DISABLED)
        self.rd_pending = False

    def on_close(self):
        if self.running:
            self.abort()
        self.master.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = MVDGui(root)
    root.mainloop()
