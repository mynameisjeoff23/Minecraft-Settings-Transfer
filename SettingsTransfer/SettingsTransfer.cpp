#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>

bool DEBUG = true;

/*this is the version of SettingsTransfer that attempts to use a struct instead of a class*/
const char *PROMPT1 = "To get started, select the \"options.txt\" file of the old minecraft version you \
    want to transfer the settings from.";

const char *PROMPT2 = "Next, select the \"options.txt\" file of the new minecraft version you want to \
    transfer the settings to. Make sure you have already installed the mods you want, and you have launced this\
    version at least once. ";

const char *PROMPT3 = "Transfer Settings!";


struct appdata{
    GtkApplication* app;
    GtkWidget *window, *nextButton, *fixed, *backButton, *view; 
    GtkFileDialog *fileDialog;
    GtkTextBuffer *buffer;

    int counter{1}, screenWidth, screenHeight, nextButtonWidth, nextButtonHeight;
    double nextButtonX, nextButtonY;

    std::string file1{""};
    std::string file2{""};

};

void wrongFile(appdata *AppData){
    GtkAlertDialog *badFile = gtk_alert_dialog_new("Warning");
    gtk_alert_dialog_set_detail(badFile, "Selected file must be \"options.txt\"");
    gtk_alert_dialog_show(badFile, GTK_WINDOW(AppData->window)); 
}

void open_file_dialog(GObject* source_object, GAsyncResult *res, gpointer user_data) {

    //why didnt I do this in python?
    appdata *AppData = static_cast<appdata*>(user_data);

    if(::DEBUG) g_print("file successfully selected\n");

    GError* error = NULL;
    GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(AppData->fileDialog), res, &error);
    if(::DEBUG) g_print("it at least gets here\n");

    if(error) {
        if(::DEBUG) g_print("Error opening file: %s\n", error->message);
        g_error_free(error);
    }
    if (file) {
        if(::DEBUG) g_print("checkpoint 3\n");
        char* path = g_file_get_path(file);
        if(::DEBUG) g_print("checkpoint 3.5\n");

        if(AppData->counter == 1) { 
            if (path) {
                AppData->file1 = std::string(path);
                g_free(path);
                if(::DEBUG) g_print("File 1: %s\n", path);
                if(::DEBUG) std::cout << "String Version: " << AppData->file1 << std::endl;

                if(AppData->file1.ends_with("options.txt")){
                    AppData->counter++;                    
                } else {
                    //makes a warning that the file must be options.txt
                    wrongFile(AppData);                   
                }
            }
        } else if(AppData->counter == 2){
            if (path) {
                AppData->file2 = std::string(path);
                g_free(path);
                if(::DEBUG) g_print("File 2: %s\n", path);
                if(::DEBUG) std::cout << "String Version: " << AppData->file2 << std::endl;

                if(AppData->file2.ends_with("options.txt")){
                    AppData->counter++;                    
                } else {
                    //makes a warning that the file must be options.txt
                    wrongFile(AppData);                   
                }
            }

        } 
            
        } else {if(::DEBUG) g_print("No file selected.\n");}
              
        // Clean up the GFile object
        g_object_unref(file);
    }


static void onButtonPress(GtkWidget *widget, gpointer user_data) {

    appdata *AppData = static_cast<appdata*>(user_data);

    AppData->fileDialog = gtk_file_dialog_new();
    

    if(::DEBUG) g_print("Hello World\n");

    if(AppData->counter == 1) {
        if(::DEBUG) g_print("option 1\n");
        gtk_file_dialog_open(AppData->fileDialog, GTK_WINDOW(AppData->window), NULL, open_file_dialog, user_data);
    }

    if(AppData->counter == 2) {
        if(::DEBUG) g_print("option 2\n");       
        gtk_file_dialog_open(AppData->fileDialog, GTK_WINDOW(AppData->window), NULL, open_file_dialog, user_data);
    }

    if(AppData->counter == 3) {

    }
}

void updateButtons(appdata* AppData){
            gtk_window_get_default_size(GTK_WINDOW(AppData->window), &AppData->screenWidth, &AppData->screenHeight);
            AppData->nextButtonWidth = AppData->screenWidth / 10;
            AppData->nextButtonHeight = AppData->screenHeight / 10;
            gtk_widget_set_size_request(AppData->nextButton, AppData->nextButtonWidth, AppData->nextButtonHeight); 

            AppData->nextButtonX = (AppData->screenWidth * .5) - (.5 * AppData->nextButtonWidth);
            AppData->nextButtonY = (AppData->screenHeight * .6) - (.5 * AppData->nextButtonHeight);
            gtk_fixed_put(GTK_FIXED(AppData->fixed), AppData->nextButton, AppData->nextButtonX, AppData->nextButtonY);
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
    
    appdata *AppData = static_cast<appdata*>(user_data);

    AppData->screenWidth = GetSystemMetrics(SM_CXSCREEN) / sqrt(2);
    AppData->screenHeight = GetSystemMetrics(SM_CYSCREEN) / sqrt(2);
    if(::DEBUG) std::cout << "things initialized" << "\nScreenWidth: " << AppData->screenWidth << std::endl;
    AppData->nextButtonWidth = AppData->screenWidth / 10;
    AppData->nextButtonHeight = AppData->screenHeight / 10;
    AppData->nextButtonX = (AppData->screenWidth * .5) - (.5 * AppData->nextButtonWidth);
    AppData->nextButtonY = (AppData->screenHeight * .6) - (.5 * AppData->nextButtonHeight);

    AppData->window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(AppData->window), "MC Settings Transfer");
    gtk_window_set_default_size( GTK_WINDOW(AppData->window), AppData->screenWidth, AppData->screenHeight);

    AppData->fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(AppData->window), AppData->fixed);

   /* if(::DEBUG) g_print("before textview\n");
    AppData->view = gtk_text_view_new();
    AppData->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(AppData->view));
    gtk_text_buffer_set_text(AppData->buffer, ::PROMPT1, -1);
    gtk_window_set_child(GTK_WINDOW(AppData->window), AppData->view);*/

    if(::DEBUG) g_print("before next button\n");
    AppData->nextButton = gtk_button_new_with_label("Select File");
    updateButtons(AppData);    
    
    g_signal_connect(AppData->nextButton, "clicked", G_CALLBACK(onButtonPress), user_data);

    if(::DEBUG) g_print("before presenting window\n");
    gtk_window_present(GTK_WINDOW(AppData->window));
 }


int main (int argc, char **argv){

    static appdata AppData;
    appdata *AppDataPtr = &AppData;

    if(::DEBUG) g_print("after defining struct\n");
    int status;
    if(::DEBUG) g_print("before new app\n");
    
    AppData.app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    if(::DEBUG) g_print("new app successful\n");
    g_signal_connect(AppData.app, "activate", G_CALLBACK (activate), AppDataPtr);
    if(::DEBUG) g_print("main runs\n");

    status = g_application_run (G_APPLICATION(AppData.app), argc, argv);
    g_object_unref(AppData.app);

    return status;
}
