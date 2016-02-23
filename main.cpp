// Brian Malloy        Game Construction
#include "menuManager.h"

int main(int, char*[]) {
   try {
      MenuManager menuManager;
      menuManager.play();
      //manager.~Manager();
        //new(&manager) Manager();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
