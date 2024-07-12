#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>

class SettingsTransfer {
    public: 

        static SettingsTransfer& getInstance() {
        // may be responsible for creating its own unique instance.
        if (nullptr == instance) instance = new SettingsTransfer;
        return *instance;
        }
        
        SettingsTransfer(const SettingsTransfer&) = delete;
        SettingsTransfer& operator=(const SettingsTransfer&) = delete;

        int getStatus(){
            return status;
        }

    private: 

        void open_file_dialog(GObject* source_object, GAsyncResult *res, gpointer user_data) {

            g_print("file successfully selected\n");
            
            if(counter == 1) {
                GError* error = NULL;
                GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source_object), res, &error);
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
                        file1 = std::string(path);
                        g_print("checkpoint 4.5\n");
                        g_free(path);
                        g_print("checkpoint 4.7");
                        g_print("Selected file:%s\n", path);
                        std::cout << "String Version:" << file1 << std::endl;

                        if(file1.ends_with("options.txt")){
                            counter++;                    
                        } else {
                            //should make a warning that the file must be options.txt
                            GtkAlertDialog *badFile = gtk_alert_dialog_new("Warning");
                            gtk_alert_dialog_set_detail(badFile, "Selected file must be \"options.txt\"");
                            gtk_alert_dialog_show(badFile, window);
                            
                        }

                    }
                    g_object_unref(file);  // Clean up the GFile object
                } else {
                    g_print("No file selected.\n");
                    
                }
            
            }
        }

        void onButtonPress(GtkWidget *widget, gpointer user_data) {
            //static std::string file1;
            //static std::string file2;

            GtkWindow* window = GTK_WINDOW(user_data);
            GtkFileDialog* dialog = gtk_file_dialog_new();
            

            g_print("Hello World\n");

            if(counter == 1) {
                g_print("option 1\n");
                gtk_file_dialog_open(dialog, window, NULL, open_file_dialog, user_data); //error on this line, probably becuase it's private
                g_print("it continues despite the function call?");
            }

            if(counter == 2) {
                g_print("option 2\n");
                

            }

            if(counter == 3) {

            }
        }

        static void activate(GtkApplication *app, gpointer user_data){
            
            screenWidth = GetSystemMetrics(SM_CXSCREEN) / sqrt(2);
            screenHeight = GetSystemMetrics(SM_CYSCREEN) / sqrt(2);
            
            int buttonWidth = screenWidth / 10;
            int buttonHeight = screenHeight / 10;

            double buttonX = (screenWidth * .5) - (.5 * buttonWidth);
            double buttonY = (screenHeight * .6) - (.5 * buttonHeight);

            window = gtk_application_window_new(app);
            gtk_window_set_title(GTK_WINDOW(window), "MC Settings Transfer");
            gtk_window_set_default_size( GTK_WINDOW(window), screenWidth, screenHeight);

            fixed = gtk_fixed_new();
            gtk_window_set_child(GTK_WINDOW(window), fixed);

            button = gtk_button_new_with_label("Select File");
            gtk_fixed_put(GTK_FIXED(fixed), button, buttonX, buttonY);
            gtk_widget_set_size_request(button, buttonWidth, buttonHeight);    
            
            g_signal_connect(button, "clicked", G_CALLBACK(onButtonPress), NULL);

            gtk_window_present(GTK_WINDOW(window));

        }



        SettingsTransfer(int argc, char **argv){

            app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
            g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

            status = g_application_run (G_APPLICATION (app), argc, argv);
            g_object_unref(app);

            //return status; //commented out, cannot return values
        }

        static SettingsTransfer *instance;

        int counter{1}, screenWidth, screenHeight, buttonWidth, buttonHeight, status;
        double buttonX, buttonY;

        std::string file1;
        std::string file2;

        GtkApplication* app;
        GtkWindow *window;
        GtkWidget *nextButton, *fixed, *backButton; 


};