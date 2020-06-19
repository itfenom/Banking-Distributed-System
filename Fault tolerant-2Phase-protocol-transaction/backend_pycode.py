from random import randint
import random
import sys
import os
import multiprocessing
import subprocess


    
    
def work_function():
	subprocess.run(command, shell=True)
       
          

if __name__ == '__main__':
	num=sys.argv[1]
	for i in range(int(num)):
		command = "gnome-terminal -- /bin/sh -c \"./backend_server "+ ";bash\""
		p = multiprocessing.Process(target=work_function)
		p.start()

    
       





