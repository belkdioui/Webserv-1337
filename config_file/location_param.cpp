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

void location_param::set_alias(std::string Alias)
{
    alias = Alias;
}

void location_param::set_methods(std::string Methods)
{
    std::string word;
    std::stringstream ss(Methods);
    std::string delim = " | "; 
    size_t pos = 0;  
    std::string token1; 
    while (( pos = Methods.find (delim)) != std::string::npos)  
    {  
        token1 = Methods.substr(0, pos);
        if(token1 == "GET")
            methods[token1] = true;
        else if (token1 == "POST")
            methods[token1] = true;
        else if (token1 == "DELETE")
            methods[token1] = true;
        else
            fun_print_error("error in this part : ", Methods);
        Methods.erase(0, pos + delim.length());
    }  
        if(Methods == "GET")
            methods[Methods] = true;
        else if (Methods == "POST")
            methods[Methods] = true;
        else if (Methods == "DELETE")
            methods[Methods] = true;
        else
            fun_print_error("error in this part : ", Methods);  
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

std::string location_param::get_alias()
{
    return alias;
}