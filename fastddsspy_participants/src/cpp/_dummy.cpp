// TODO remove

int clang_should_fail()
{
    int i = 0;
    if ((i = 1) == true)
    {
        return 0;
    }
    return 1;
}

int uncrustify_should_fail(){
    return 0;
}
