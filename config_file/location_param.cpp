#include "location_param.hpp"

location_param::location_param()
{

}

location_param::~location_param()
{
    
}
bool location_param::operator==(const location_param& other)
{
	if (redirect_url == other.redirect_url && methods == other.methods && index == other.index && directory_listing == other.directory_listing && upload_dir == other.upload_dir && root == other.root && alias == other.alias)
		return true;
	return false;
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
    Methods = utils::delete_all_whitespace_and_set_one_space(Methods);
    std::string word;
    std::stringstream ss(Methods);
    while(!ss.eof())
    {
        std::getline(ss, word, ' ');
        if(word == "GET" && !get_methods("GET"))
            methods[word] = true;
        else if (word == "POST" && !get_methods("POST"))
            methods[word] = true;
        else if (word == "DELETE" && !get_methods("DELETE"))
            methods[word] = true;
        else
            utils::print_error("error in this part : ", word); 
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

std::string location_param::get_alias()
{
    return alias;
}