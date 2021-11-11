from tkinter import *
import socket
import json
import threading

# we need to use the ip of the PI running atlas
host = "192.168.137.75"
# we are supposed to use this port
port = 6668
#alarm mode global variable of pushbutton
pushbuttonmode = 1

def center(win): #centers a tkinter window with parameter of the main window
    win.update_idletasks()
    width = win.winfo_width()
    frm_width = win.winfo_rootx() - win.winfo_x()
    win_width = width + 2 * frm_width
    height = win.winfo_height()
    titlebar_height = win.winfo_rooty() - win.winfo_y()
    win_height = height + titlebar_height + frm_width
    x = win.winfo_screenwidth() // 2 - win_width // 2
    y = win.winfo_screenheight() // 2 - win_height // 2
    win.geometry('{}x{}+{}+{}'.format(width, height, x, y))
    win.deiconify()
    
def pushbuttonclick():
    pushbuttonBox.delete(0, 'end')
    s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s1.connect((host, port))
    #connection confirmation
    print("connected ", s1)
    global pushbuttonmode
    #muted and unmuted calls
    if pushbuttonmode == 1:
        print("1")
        togglecallasync = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "Toggle", "Service Inputs" : "(1)"}'
    if pushbuttonmode == 2:
        print("2")
        togglecallasync = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "Toggle", "Service Inputs" : "(2)"}'
    #send
    s1.sendall(togglecallasync)
    #receive
    pushdata = s1.recvfrom(1024)
    pushdata = pushdata[0].decode('utf-8')
    print('Received ' + str(pushdata) + "\n")
    pushbutton12 = json.loads(pushdata)
    pushbuttonmode = pushbutton12["Service Result"]
    pushbuttonmode = int(pushbuttonmode)
    if pushbuttonmode == 1:
        pushbuttonBox.insert(END, 'Muted')
    else:
        pushbuttonBox.insert(END, 'Unmuted')
    s1.close()
        
 
def inchesbuttonclick(): #receives inches value from atlas and displays service result from the json string
    inchesBox.delete(0, 'end')
    # invoke service
    call1 = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "DistanceInch", "Service Inputs" : "()"}'
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    #connection confirmation
    print("connected ", s)
    s.sendall(call1)
    #await response
    data = s.recvfrom(1024)
    s.close()
    data = data[0].decode('utf-8')

    # response
    print('Received ' + str(data) + "\n")
    
    distInch = json.loads(data)
    inches = distInch["Service Result"]
    if int(inches) < 5: #start alarm if distance too close
        #call to start alarm
        callnormalarm = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "NormalAlarm", "Service Inputs" : "()"}'
        callmutealarm = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "MuteAlarm", "Service Inputs" : "()"}'
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))
        #connection confirmation
        print("connected ", s)
        if pushbuttonmode == 1:
            s.sendall(callmutealarm)
        else:
            s.sendall(callnormalarm)
    inchesBox.insert(END, str(inches))
    
def cmbuttonclick(): #receives centimeters value from atlas and displays service result from the json string
    cmBox.delete(0, 'end')
    # invoke service
    call2 = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "DistanceCM", "Service Inputs" : "()"}'
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    #connection confirmation
    print("connected ", s)
    s.sendall(call2)
    #await response
    data = s.recvfrom(1024)
    s.close()
    data = data[0].decode('utf-8')

    # response
    print('Received ' + str(data) + "\n")
    
    distCm = json.loads(data)
    cms = distCm["Service Result"]
    if int(cms) < 13: #start alarm if distance too close
        #call to start alarm
        callnormalarm = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "NormalAlarm", "Service Inputs" : "()"}'
        callmutealarm = b'{"Tweet Type" : "Service call", "Thing ID" : "MySmartThing01", "Space ID" : "MySmartSpace", "Service Name" : "MuteAlarm", "Service Inputs" : "()"}'
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))
        #connection confirmation
        print("connected ", s)
        if pushbuttonmode == 1:
            s.sendall(callmutealarm)
        else:
            s.sendall(callnormalarm)
    cmBox.insert(END, str(cms))

#makes a tkinter gui
mainWindow = Tk()
mainWindow.title("Atlas Services")
mainWindow.geometry("600x400")
center(mainWindow)
windowTitle = Label(mainWindow, text = "Atlas Middleware Services", font=("Helvetica", 18, 'bold'))
windowTitle.place(x = 150, y = 20)

#makes an inches button and entrybox
inchesButton = Button(mainWindow, text = "Distance (in.)", font=("Helvetica", 12), width = 12, bg='#00a2ff', activebackground='#00a2ff', command = inchesbuttonclick)
inchesButton.place(x = 160, y = 90)
inchesBox = Entry(mainWindow, justify='center', font=("Helvetica", 16),  width = 12)
inchesBox.place(x = 300, y = 93)
inchesBox.insert(END, '-')

#makes a centimeters button and entrybox
cmButton = Button(mainWindow, text = "Distance (cm.)", font=("Helvetica", 12), width = 12, bg='#00a2ff', activebackground='#00a2ff', command = cmbuttonclick)
cmButton.place(x = 160, y = 180)
cmBox = Entry(mainWindow, justify='center', font=("Helvetica", 16),  width = 12)
cmBox.place(x = 300, y = 183)
cmBox.insert(END, '-')

#makes a pushbutton button and entrybox
pushbuttonButton = Button(mainWindow, text = "Alarm", font=("Helvetica", 12), width = 12, bg='#00a2ff', activebackground='#00a2ff', command = pushbuttonclick)
pushbuttonButton.place(x = 160, y = 270)
pushbuttonBox = Entry(mainWindow, justify='center', font=("Helvetica", 16),  width = 12)
pushbuttonBox.place(x = 300, y = 273)
pushbuttonBox.insert(END, 'Muted')

#displays the tkinter gui
mainWindow.mainloop()