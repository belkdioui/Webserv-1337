#include "utils.hpp"

namespace utils {
    std::string big_index = "";
}

std::string utils::delete_all_whitespace_and_set_one_space(std::string str)
{
    std::string word;
    for(unsigned long i = 0; i < str.size(); i++)
    {
        if(i == 0 && (str[i] == ' ' || str[i] == '\t'))
        {
            while((str[i] == ' ' || str[i] == '\t') && i < str.size())
                i++;
            if(i > 0)
                i--;
        }
        else
        {
            while(i < str.size() && str[i] != ' ' && str[i] != '\t')
            {
                word+=str[i];
                i++;
            }
            if(i < str.size() && (str[i] == ' ' || str[i] == '\t'))
            {
                while((str[i] == ' ' || str[i] == '\t') && i < str.size())
                    i++;
                if(i < str.size())
                {
                    i--;
                    word+=' ';
                }
            }
        }
    }
    return word;
}

void utils::print_error(std::string str, std::string str1)
{
    std::cout<<str<<std::endl;
    std::cout<<"**"<<str1<<"**"<<std::endl;
    exit(1);
}

std::string utils::trim_space_from_back(std::string str) {
    int i = str.size();
    while (i > 0 && (str[i - 1] == ' ' || str[i - 1] == '\t' || str[i - 1] == '\n')) {
        i--;
    }
    return std::string(str.substr(0, i));
}

std::string normalizeSlashes1(const std::string &input) {
    std::string result;
    bool lastWasSlash = false;

    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (*it == '/') {
            if (!lastWasSlash) {
                result += *it;
                lastWasSlash = true;
            }
        } else {
            result += *it;
            lastWasSlash = false;
        }
    }

    return result;
}

int utils::deletePath(const char* path) 
{
    if (std::remove(normalizeSlashes1(path).c_str()) == 0)
        return 0;
    return -1;
}

int utils::removeDirectoryRecursively(const char *path) 
{
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(normalizeSlashes1(path).c_str())) == NULL) {
        std::cout << "failed to opendir" << std::endl;
        return -1;
    }
    while ((entry = readdir(dir))!= NULL) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::cout << entry->d_name << std::endl;
            if (entry->d_type == DT_DIR) {
                return removeDirectoryRecursively(entry->d_name);
            } else {
                std::string full_path = std::string(path) + std::string(entry->d_name);
                if(std::remove(full_path.c_str()) != 0 )
                {
                    return -1;
                    std::cout << "std::remove failed" << std::endl;
                }
            }
        }
    }
    closedir(dir);
    if(std::remove(normalizeSlashes1(path).c_str()))
        return -1;
    return 0;
}