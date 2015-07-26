using func_p = void(*)();

//Labels to the arrays of constructors and destructors placed by the linker
extern "C" func_p start_ctors, end_ctors, start_dtors, end_dtors;

//Constructs all static objects
void static_construct()
{
    for(auto fp = &start_ctors; fp < &end_ctors; ++fp)
        (*fp)();
}

//Destructs all static objects
void static_destruct()
{
    for(auto fp = &end_dtors-1; fp >= &start_dtors; --fp)
        (*fp)();
}
