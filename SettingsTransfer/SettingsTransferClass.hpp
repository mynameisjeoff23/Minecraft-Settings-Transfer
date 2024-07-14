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

        GtkWindow getWindow(){return window;}

        void setApp(GtkApplication *structure) {app = structure;}

        GtkApplication getApp(){return app;}

        void setNextButton(GtkWidget *structure) {nextButton = structure;}

        GtkWidget getNextButton(){return nextButton;}

        void setFixed(GtkWidget *structure) {fixed = structure;}

        GtkWidget getFixed(){return fixed;}

        void setBackButton(GtkWidget *structure) {backButton = structure;}

        GtkWidget getBackButton(){return backButton;}

        void setFile1(std::string object) {file1 = object;}

        std::string getFile1(){return file1;}

        void setFile2(std::string object) {file2 = object;}

        std::string getFile2(){return file1;}

        void setNextButtonPos(double x, double y){
            nextButtonY = y;
            nextButtonX = x;
        }

        double getNextButtonX(return nextButtonX;)

        double getNextButtonY(return nextButtonY)

        void addCounter() {if(counter < 3) counter++;}

        void subCounter() {if(counter > 1) counter--;}

        int getCounter() {return counter;}

        void setScreenSize(int height, int width){//doesnt really work here
            screenHeight = height;
            screenWidth = width;
        }

        void setNextButtonSize(int height, int width){
            nextButtonWidth = width;
            nextButtonHeight = height;
        }

    private: 

        SettingsTransfer(){}

        int counter{1}, screenWidth, screenHeight, nextButtonWidth, nextButtonHeight, status;
        double nextButtonX, nextButtonY;

        std::string file1;
        std::string file2;

        GtkApplication* app;
        GtkWindow *window;
        GtkWidget *nextButton, *fixed, *backButton; 


};