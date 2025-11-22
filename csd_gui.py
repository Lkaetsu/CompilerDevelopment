import tkinter as tk
from tkinter import filedialog, messagebox
from textLineNumbers import CustomText, TextLineNumbers
import threading
import subprocess
import queue
import os

class CSDGUI:
    def __init__(self, master):
        self.master = master
        master.title("CSD Interface")

        self.proc = None
        self.stdout_queue = queue.Queue()
        self.running = False
        self.saved = False

        # Top frame for file selection
        top = tk.Frame(master)
        top.pack(fill=tk.X, padx=8, pady=4)

        self.file_var = tk.StringVar()
        tk.Label(top, text="Arquivo de Código:").pack(side=tk.LEFT)
        tk.Entry(top, textvariable=self.file_var, width=50).pack(side=tk.LEFT, padx=4)
        tk.Button(top, text="Procurar", command=self.select_file).pack(side=tk.LEFT)

        # Buttons
        mode_frame = tk.Frame(master)
        mode_frame.pack(fill=tk.X, padx=8, pady=4)
        tk.Button(mode_frame, text="Salvar", command=self.save).pack(side=tk.LEFT)
        tk.Button(mode_frame, text="Compilar", command=self.compile).pack(side=tk.LEFT)

        # File area
        self.file = CustomText(master, height=25, wrap="none")
        self.file.pack(fill=tk.BOTH, expand=True, padx=8, pady=4)
        self.linenumbers = TextLineNumbers(master, width=30)
        self.linenumbers.attach(self.file)
        self.linenumbers.pack(side=tk.LEFT, fill="y")
        self.file.bind("<<Change>>", self._on_change)
        self.file.bind("<<Configure>>", self._on_change)

        # Log area
        self.text = tk.Text(master, height=25, wrap="none")
        self.text.pack(fill=tk.BOTH, expand=True, padx=8, pady=4)
        self.text.configure(state=tk.DISABLED)

        # Scrollbars
        self.yscrollfile = tk.Scrollbar(self.file, orient=tk.VERTICAL, command=self.file.yview)
        self.file.configure(yscrollcommand=self.yscrollfile.set)
        self.yscrollfile.pack(side=tk.RIGHT, fill=tk.Y)

        self.yscroll = tk.Scrollbar(self.text, orient=tk.VERTICAL, command=self.text.yview)
        self.text.configure(yscrollcommand=self.yscroll.set)
        self.yscroll.pack(side=tk.RIGHT, fill=tk.Y)

        master.protocol("WM_DELETE_WINDOW", self.on_close)

    def _on_change(self, event):
        self.linenumbers.redraw()

    def log(self, line, mode="rep"):
        self.text.configure(state=tk.NORMAL)
        if mode == "rep":
            self.text.replace("1.0", tk.END, line + "\n")
        elif mode == "cat":
            self.text.insert(tk.END, line + "\n")
        self.text.see(tk.END)
        self.text.configure(state=tk.DISABLED)

    def select_file(self):
        path = filedialog.askopenfilename(title="Selecione um arquivo de código", filetypes=[("Todos","*.*")])
        if path:
            self.file_var.set(path)
            with open(path, "r") as file:
                self.file.replace("1.0",tk.END, file.read())

    def start_process(self):
        if self.proc is not None:
            messagebox.showerror("Erro", "Processo já em execução.")
            return False
        file_path = self.file_var.get().strip()
        if not file_path:
            messagebox.showerror("Erro", "Selecione o arquivo de código")
            return False
        if not os.path.exists(file_path):
            messagebox.showerror("Erro", "Arquivo não encontrado.")
            return False
        exe = self.find_executable()
        if exe is None:
            messagebox.showerror("Erro", "Executável csd não encontrado. Compile primeiro.")
            return False
        cmd = [exe, file_path]
        try:
            self.proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1)
            self.master.after(100, self.poll_stdout)
        except Exception as e:
            messagebox.showerror("Erro", f"Falha ao iniciar processo: {e}")
            self.proc = None
            return False
        self.running = True
        threading.Thread(target=self.read_stdout, daemon=True).start()
        return True

    def find_executable(self):
        # procura por csd.exe ou csd (Linux) no diretório atual
        candidates = ["csd.exe", "csd"]
        for c in candidates:
            if os.path.exists(c):
                return c
        return None

    def compile(self):
        if not self.start_process():
            return
        self.log("Compilando...")

    def save(self):
        file_path = self.file_var.get().strip()
        if not os.path.exists(file_path):
            try:
                with filedialog.asksaveasfile(title="Salvar como", filetypes=[("Todos","*.*")]) as file_path:
                    self.file_var.set(file_path.name)
                    file_path.write(self.file)
            except TypeError as e:
                return
        else:
            with open(file_path, "w") as file:
                file.write(self.file.get("1.0",tk.END))
        
        self.saved = True
        self.log("Salvo com sucesso.")


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
                self.log(line)
        except queue.Empty:
            pass
        if self.running:
            self.master.after(100, self.poll_stdout)
        else:
            self.proc = None
            self.log("------------------------\nCompilação finalizada.", mode="cat")

    def force_kill(self):
        if self.running and self.proc:
            try:
                self.proc.kill()
            except Exception:
                pass
            self.running = False
    
    def on_close(self):
        self.master.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = CSDGUI(root)
    root.mainloop()
