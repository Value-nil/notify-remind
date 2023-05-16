#include <iostream>
#include <notify.h>
#include <fcntl.h> 
#include <string>
#include <vector>
#include <map>

using namespace std;

void sendNotification(){
    notify_init("THE app");

    NotifyNotification* notify = notify_notification_new("Levanta la vista por 20 segundos", "", "");
    
    GError** someError;
    gboolean success = notify_notification_show(notify, someError);
    if(!success){
        cerr << (**someError).message;
    }


    notify_uninit();
}
vector<string> slice(string toSlice, char slicing){
    vector<string> toReturn;
    string temp = "";
    for(int i = 0; i < toSlice.length(); i++){
        if(toSlice[i] == slicing){
            toReturn.push_back(temp);
            temp = "";
        }
        else{
            temp+=toSlice[i];
        }
    }
    if(temp != ""){
        toReturn.push_back(temp);
    }
    return toReturn;
}



int main(){
    int filedes = open("/etc/notifyReminderConfig.dat", O_RDWR, O_CREAT);
    char buff[32];
    map<string, string> settings = {
        {"SUMMARY", "Levanta la vista por 20 segundos!"},
        {"BODY", "Hazlo"},
        {"APP_NAME", "THE app"},
        {"EXPIRE_TIME", "-1"},
        {"NOTIFICATION_URGENCY", "NOTIFY_URGENCY_NORMAL"}
    };
    read(filedes, buff, 32);
    cout << buff << '\n';
    vector<string> result = slice(buff, '\n');
    for(int i = 0; i < result.size(); i++){
        cout << result[i] << ' ';
    }
    char vuff[] = "";
    write(filedes, vuff, sizeof(vuff));
    close(filedes);
    return 0;
}