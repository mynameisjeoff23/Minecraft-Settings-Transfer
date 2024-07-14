#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>

class SettingsTransfer {
    public: 

        static SettingsTransfer& getInstance() {
            static SettingsTransfer instance; // Guaranteed to be destroyed and instantiated on first use
            return instance;
        }
        SettingsTransfer(const SettingsTransfer&) = delete;
        SettingsTransfer& operator=(const SettingsTransfer&) = delete;

        int getStatus(){
            return status;
        }

        void setWindow(GtkWindow *object){
            window = object;
        }

    private: 

        SettingsTransfer(){}

        int counter{1}, screenWidth, screenHeight, buttonWidth, buttonHeight, status;
        double buttonX, buttonY;

        std::string file1;
        std::string file2;

        GtkApplication* app;
        GtkWindow *window;
        GtkWidget *nextButton, *fixed, *backButton; 


};