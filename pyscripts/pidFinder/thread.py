import threading
 
class MyThread(threading.Thread):
    def __init__(self, func, *args):
        threading.Thread.__init__(self)
        self.on = True
        self.func = func
        self.args = args
    
    def stop(self):
        self.on = False
        
    def run(self):
        self.on = True
        while(self.on):
            self.func(self.args)
