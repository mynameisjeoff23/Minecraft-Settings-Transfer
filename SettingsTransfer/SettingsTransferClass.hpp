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

        void setStatus(int i) {status = i;}

        int getStatus() {return status;}

        void setWindow(GtkWindow *structure) {window = structure;}

        void setApp(GtkApplication *structure) {app = structure;}

        void setNextButton(GtkWidget *structure) {nextButton = structure;}

        void setFixed(GtkWidget *structure) {fixed = structure;}

        void setBackButton(GtkWidget *structure) {backButton = structure;}

        void setFile1(std::string object) {file1 = object;}

        void setFile1(std::string object) {file2 = object;}

        void setButtonPos(double x, double y){
            buttonX = x;
            buttonY = y;
        }

        void addCounter() {counter++;}

        void subCounter() {counter--;}

        void setScreenSize(int height, int width){
            screenHeight = height;
            screenWidth = width;
        }

        void setButtonSize(int height, int width){
            buttonWidth = width;
            buttonHeight = height;
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