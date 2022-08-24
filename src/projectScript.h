//--------------------------------------------------
// Game of life
// projectScript.h
// Date: 2022-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/pch.h>
#include <atta/script/projectScript.h>
#include <atta/resource/resources/image.h>

class Project : public atta::script::ProjectScript {
  public:
    void onLoad() override;

    void onStart() override;
    void onStop() override;

    void onUpdateBefore(float dt) override;

  private:
    void init();
    void clear();

    atta::resource::Image* _gofImage;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
