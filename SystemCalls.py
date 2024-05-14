import os 
import multiprocessing

def child_process(file_name):
    child_pid = os.getpid()
    print(f"Child PPID: ", child_pid)
    
    #read by child process
    with open(file_name, 'r') as file:
        data = file.read()
        print(f"Child reading: " + data + '\n')
    
    #WRITE BY CHILD 
    with open (file_name, "a") as file:
        print("child writing \n")
        file.write(f"Child {child_pid} writing ... \n")
        
    #change permission 
    # os.chmod(file, 0o600)
    # print("file permission changed")
    
        

def parent_process(file_name):
    parent_pid = os.getpid()
    print("Parent PID: ", parent_pid)
    
    #create child process
    p = multiprocessing.Process(target = child_process, args = (file_name,))
    p.start()
    p.join()
    
    #read from file 
    with open(file_name, 'r') as file:
        data = file.read()
        print("data in file before parent writing: " + data)
        print('\n')
        
    #write by parent 
    with open(file_name, 'a') as file:
        file.write(f"Parent {parent_pid} writing.....\n")
    
    #read by parent 
    with open(file_name, 'r') as file :
        data = file.read()
        print(f"parent {parent_pid} read :" + data)
    
    #change permission 
    # os.chmod(file, 0o600)
    # print("file permission changed to 600.")
    
def main():
    file_name = "demo_file.txt"
    with open(file_name, 'w') as f:
        f.write("Initial Content by main\n")
    
    parent_process(file_name)

if __name__ == "__main__":
    main()