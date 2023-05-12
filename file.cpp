#include <iostream>
#include <notify.h>


int main(){
    notify_init("THE app");

    NotifyNotification* notify = notify_notification_new("Levanta la vista por 20 segundos", "", "");
    
    GError** someError;
    gboolean success = notify_notification_show(notify, someError);
    if(!success){
        std::cerr << (**someError).message;
    }


    notify_uninit();
    std::cout << "Success";
    return 0;
}