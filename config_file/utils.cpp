#include "utils.hpp"

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