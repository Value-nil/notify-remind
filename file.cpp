#include <iostream>
#include <notify.h>

using namespace std;


int main(){
    notify_init("THE app");

    NotifyNotification* notify = notify_notification_new("Levanta la vista por 20 segundos", "", "");
    
    GError* someError = nullptr;
    gboolean success = notify_notification_show(notify, &someError);
    if(!success){
        cerr << (*someError).message;
    }


    notify_uninit();
    return 0;
}