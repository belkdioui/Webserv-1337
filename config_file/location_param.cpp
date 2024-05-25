#include "location_param.hpp"

void fun_print_error(std::string str, std::string str1)
{
    std::cout<<str<<std::endl;
    std::cout<<str1<<std::endl;
    exit(1);
}

location_param::location_param()
{

}

location_param::~location_param()
{
    
}

void location_param::set_redirect_url(std::string Redirect_url)
{
    redirect_url = Redirect_url;
}

void location_param::set_methods(std::string Methods)
{
    std::string word;
    std::stringstream ss(Methods);
    while (!ss.eof())
    {
        std::getline(ss, word,  '|');
        if(word == "GET")
            methods[word] = true;
        else if (word == "POST")
            methods[word] = true;
        else if (word == "DELETE")
            methods[word] = true;
        else
            fun_print_error("error in this part : ", Methods);
    }
    
}

void location_param::set_index(std::string Index)
{
    index = Index;
}
void location_param::set_root(std::string Root)
{
    root = Root;
}

void location_param::set_directory_listing(std::string Directory_listing)
{
    directory_listing = Directory_listing;
}

void location_param::set_upload_dir(std::string Upload_dir)
{
    upload_dir = Upload_dir;
}

std::string location_param::get_redirect_url()
{
    return redirect_url;
}

bool location_param::get_methods(std::string key)
{
    std::map<std::string, bool>::iterator it = methods.find(key);
    if(it == methods.end())
        return false;
    return it->second;
}

std::string location_param::get_index()
{
    return index;
}

std::string location_param::get_directory_listing()
{
    return directory_listing;
}

std::string location_param::get_upload_dir()
{
    return upload_dir;
}

std::string location_param::get_root()
{
    return root;
}