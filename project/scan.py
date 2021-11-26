import socket
import struct
import threading
import time
import json

def serviceScan(service_jsons):
    # these are the ip and ports we need for the atlas multicast
    multicast_group = '232.1.1.1'
    server_address = ('', 1235)

    # create and bind socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(server_address)

    # assign and set up group
    group = socket.inet_aton(multicast_group)
    mreq = struct.pack('4sL', group, socket.INADDR_ANY)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    # scan for services for 60 seconds
    start = time.time()
    while time.time() - start < 60:
        print('\nwaiting to receive message')
        data, address = sock.recvfrom(1024)
        print('received %s bytes from %s' % (len(data), address))
        print(data)
        service_jsons.append(data)


def duplicateCheck(compare_list, thing_id, service_name):
    for i in compare_list:
        if i["Thing ID"] == thing_id:
            return True
        if i["Service Name"] == thing_id:
            return True
    return False
    
    
# gather services from the received data (currently do not remove dups)
json_service_list = []
serviceScan(json_service_list)

print("This is the list of JSON data:\n")
service_list = []
for j in json_service_list:
    index = j.find("[")
    if index != -1:
        front = j[:index]
        middle = j[index+1:]
        index = middle.find("]")
        rear = middle[index+1:]
        middle = middle[:index]
        print("Removed: " + middle)
        j = front + rear
    print(j["Name"] + "\n")
    x = json.loads(j)
    if x["Tweet Type"] == "Service":
        service_list.append(x)

# print service names
print("\n\n\n\nServices: ")
services = []
for j in service_list:
    new_json = {
                "Tweet Type" : "Service call",
                "Thing ID" : j["Thing ID"],
                "Space ID" : j["Space ID"],
                "Service Name" : j["Name"],
                "Service Inputs" : "placeholder"
    }
    if duplicateCheck(services, j["Thing ID"], j["Name"]) == False:
        services.append(new_json)
print(services)

# things i need:
# tweet type
# thing id
# space id
# service names
# service inputs