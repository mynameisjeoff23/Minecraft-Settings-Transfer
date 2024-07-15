#include "SettingsTransferClass.hpp"



void open_file_dialog(GObject* source_object, GAsyncResult *res, gpointer user_data) {

    SettingsTransfer *AppData = static_cast<SettingsTransfer*>(user_data);

    g_print("file successfully selected\n");
    
    if(AppData->getCounter() == 1) {
        GError* error = NULL;
        GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(AppData->getDialog()), res, &error);
        g_print("it at least gets here\n");

        if(error) {
            g_print("Error opening file: %s\n", error->message);
            g_error_free(error);
        }
        if (file) {
            g_print("checkpoint 3\n");
            char* path = g_file_get_path(file);
            g_print("checkpoint 3.5\n");

            if (path) {
                g_print("checkpiont 4\n");
                AppData->setFile1(std::string(path));
                g_print("checkpoint 4.5\n");
                g_free(path);
                g_print("checkpoint 4.7");
                g_print("Selected file:%s\n", path);
                std::cout << "String Version:" << AppData->getFile1() << std::endl;

                if(AppData->getFile1().ends_with("options.txt")){
                    AppData->addCounter();                    
                } else {
                    //should make a warning that the file must be options.txt
                    GtkAlertDialog *badFile = gtk_alert_dialog_new("Warning");
                    gtk_alert_dialog_set_detail(badFile, "Selected file must be \"options.txt\"");
                    gtk_alert_dialog_show(badFile, GTK_WINDOW(AppData->getWindow()));
                    
                }

            }
            g_object_unref(file);  // Clean up the GFile object
        } else {
            g_print("No file selected.\n");
            
        }
    
    }
}

static void onButtonPress(GtkWidget *widget, gpointer user_data) {

    SettingsTransfer *AppData = static_cast<SettingsTransfer*>(user_data);
    std::cout << "Counter: " << AppData->getCounter() << "\nfile1: " << AppData->getFile1() <<\
    "\nwindow width: " << AppData->getScreenWidth() << "\nbutton x: " << AppData->getNextButtonX() << std::endl;


    AppData->setDialog(gtk_file_dialog_new());
    

    g_print("Hello World\n");

    if(AppData->getCounter() == 1) {
        g_print("option 1\n");
        gtk_file_dialog_open(AppData->getDialog(), GTK_WINDOW(AppData->getWindow()), NULL, open_file_dialog, &AppData);
        g_print("it continues despite the function call?");
    }

    if(AppData->getCounter() == 2) {
        g_print("option 2\n");
        

    }

    if(AppData->getCounter() == 3) {

    }
}

/* there will be no button updating until I figure out a way to detect a resize that doesn't crash the program

gboolean update_button(gpointer user_data){
    GtkWindow *window = GTK_WINDOW(user_data)

    int windowWidth, windowHeight;
    gtk_window_get_default_size(GTK_WINDOW(window), &width, &height);

    int buttonWidth = windowWidth / 10;
    int buttonHeight = windowHeight / 10; 

    gtk_widget_set_size_request(button, buttonWidth, buttonHeight);
}

    int buttonX = windowWidth * .6;
    int buttonY = windowHeight * .5;

    gtk_fixed_put(GTK_FIXED(fixed), button, buttonX, buttonY);
} */
static void activate(GtkApplication *app, gpointer user_data){
    
    SettingsTransfer *AppData = static_cast<SettingsTransfer*>(user_data);

    AppData->setWindow(gtk_application_window_new(app));

    gtk_window_set_title(GTK_WINDOW(AppData->window), "MC Settings Transfer");
    gtk_window_set_default_size( GTK_WINDOW(AppData->window), AppData->getScreenWidth(), AppData->getScreenHeight());

    AppData->setFixed(gtk_fixed_new());
    gtk_window_set_child(GTK_WINDOW(AppData->getWindow()), AppData->getFixed());

    AppData->setNextButton(gtk_button_new_with_label("Select File"));
    AppData->updateButtons();    
    
    g_signal_connect(AppData->getNextButton(), "clicked", G_CALLBACK(onButtonPress), &AppData);

    gtk_window_present(GTK_WINDOW(AppData->getWindow()));

    AppData->setFile1("");

}


int main (int argc, char **argv){
    SettingsTransfer &AppData = SettingsTransfer::getInstance();

    GtkApplication *app;
    g_print("after defining class\n");
    int status;
    g_print("before new app\n");
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    AppData.setApp(app);
    g_print("new app successful\n");
    g_signal_connect(app, "activate", G_CALLBACK (activate), &AppData);
    g_print("main runs\n");

    status = g_application_run (G_APPLICATION(app), argc, argv);
    g_object_unref(AppData.getApp());

    return status;
}
