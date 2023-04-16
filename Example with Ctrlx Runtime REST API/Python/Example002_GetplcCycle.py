import requests
import sys

#The base URL that you can find in the API Reference
"""
    please find more information from this link:
    https://developer.community.boschrexroth.com/t5/Store-and-How-to/Using-REST-API-of-ctrlX-CORE/ba-p/12490

    VirtualCore is used in the example:127.0.0.1:8443 is the default URL.

    Authorization Link:https://127.0.0.1:8443/identity-manager/api/v1/auth/token
    CTRLX REST Basic Link:https://127.0.0.1:8443/automation/api/v1

    you Should get the response from CtrlX Virtual Core like this:
        key:access_token , value:XXXXXXXXXXXXXXXXXXXXXXXXXXX
        key:token_type , value:Bearer

    Script Result:
    key:info , value:[
        {'name': 'MainTask', 'priority': 30, 'watchdog': False, 
        'watchdogTime': 0, 'cycleTime': 24, 'averageCycleTime': 60, 
        'maxCycleTime': 15280, 'minCycleTime': 9, 'cycleCount': 2042, 'watchdogSensitivity': 1,
          'interval': 200000, 'taskType': 'CYCLIC', 'osHandle': 139832806885120}]

"""
USERNAME='"boschrexroth"'
PASSWORD='"boschrexroth"'

#Url Configuration
Auth_URL='https://127.0.0.1:8443/identity-manager/api/v1/auth/token'
CTRLX_URL_Basic='https://127.0.0.1:8443/automation/api/v1/'
    

#Header Configuration
#Additionally the "Content-Type" "application/json" has to be added to the request header to tell the server how data should be interpreted.
headers={'Content-type': 'application/json'}

#Post Content
postData='''
{"name":%s,"password":%s}
'''%(USERNAME,PASSWORD)

#Send Request
r=requests.post(Auth_URL,postData,verify=False,headers=headers)

#Get Status Code
#if 201 is return, it means the request is done without error.
print('Status Code=%s'%(r.status_code))
data=r.json()

if r.status_code != 201:    
    for k,v in data.items():
        print('key:%s , value:%s'%(k,v))
    print('Exit..')
    sys.exit();

#
AuthHeader={'Content-type': 'application/json','Authorization':'Bearer '+data['access_token']}
r=requests.get(
    CTRLX_URL_Basic+'%s'%('/plc/app/Application/admin/task/')
    ,verify=False
    ,headers=AuthHeader)
data=r.json()
for k,v in data.items():
    print('key:%s , value:%s'%(k,v))