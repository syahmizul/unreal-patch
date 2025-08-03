#include <Mod/CppUserModBase.hpp>
#include <include/Globals.hpp>

class MyMod : public CppUserModBase
{
public:
    MyMod() : CppUserModBase() {
        ModName = STR("MyMod");
        ModVersion = STR("1.0");
        ModDescription = STR("Unreal Engine signature bypass.");
        ModAuthors = STR("Syahmi Zulkalnain");

        Globals::Initialize();
    }
    ~MyMod() override {
        Globals::Uninitialize();
    }
};

// Export functions
extern "C"
{
    __declspec(dllexport) CppUserModBase* start_mod()
    {
        return new MyMod();
    }

    __declspec(dllexport) void uninstall_mod(CppUserModBase* mod)
    {
        delete mod;
    }
}