#include "yoyo_application.hpp"

int main(int argc, char** argv)
{
  yoyo::yoyo_application app(argc, argv);

  app.setup();

  return app.exec();
}
