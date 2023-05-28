#include <iostream>
#include <notify.h>
#include <fcntl.h> 
#include <string>
#include <vector>
#include <map>
#include <errno.h>
#include <sys/stat.h>

using namespace std;

map<string, string> settings = {
        {"SUMMARY", "Levanta la vista por 20 segundos!"},
        {"BODY", "Hazlo"},
        {"APP_NAME", "THE app"},
        {"EXPIRE_TIME", "-1"},
        {"NOTIFICATION_URGENCY", "NOTIFY_URGENCY_NORMAL"}
};


void sendNotification(){
    notify_init(settings["APP_NAME"].c_str());

    NotifyNotification* notify = notify_notification_new(settings["SUMMARY"].c_str(), settings["BODY"].c_str(), "");
    int timeout;
    try{
        timeout = stoi(settings["EXPIRE_TIME"]);
    }
    catch(invalid_argument){
        std::cerr << "Invalid number";
        timeout = -1;
    }
    GError** someError = nullptr;
    notify_notification_set_timeout(notify, timeout);
    gboolean success = notify_notification_show(notify, someError);
    if(!success){
        cerr << (**someError).message;
    }


    notify_uninit();
}
vector<string> slice(string toSlice, char slicing, int limit){
    vector<string> toReturn;
    string temp = "";
    int sliced = 1;
    for(int i = 0; i < toSlice.length(); i++){
        if(toSlice[i] == slicing && sliced != limit){
            toReturn.push_back(temp);
            temp = "";
            sliced++;
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

int getFileSize(){
    struct stat* fileStat = new struct stat;
    int success = stat("notifyReminderConfig.dat", fileStat);
    int size = fileStat->st_size;
    delete fileStat;
    if(success == -1){
        std::cerr << errno;
        return -1;
    }
    
    else{
        return size;
    }
}




void processFile(char* read){
    vector<string> result = slice(read, '\n', 0);
    for(int i = 0; i < result.size(); i++){
        vector<string> result2 = slice(result[i], '=', 2);
        if(result2[1] != "" && settings[result2[0]] != ""){
            settings[result2[0]] = result2[1];
            std::cout << result2[0] + " is being set to " + result2[1] << '\n';
        }
    }
}



int main(){
    int filedes = open("notifyReminderConfig.dat", O_RDWR | O_CREAT);
    if(filedes == -1){
        std::cerr << errno;
        return -1;
    }

    int size = getFileSize();
    if(size == -1) return -1;
    char* buff = new char[size+1];
    read(filedes, buff, size);
    buff[size] = '\0';
    cout << buff << '\n';
    processFile(buff);

    sendNotification();
    
    close(filedes);
    return 0;
}