#include "location_param.hpp"

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
    methods = Methods;
}

void location_param::set_index(std::string Index)
{
    index = Index;
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

std::string location_param::get_methods()
{
    return methods;
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