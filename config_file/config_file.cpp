#include "config_file.hpp"

#include <iterator>

void config_file::check_if_duplicate_server()
{
    for (size_t i = 0; i < servers.size(); ++i) {
        for (size_t j = i + 1; j < servers.size(); ++j) {
            if (servers[i].get_host() == servers[j].get_host() && 
                servers[i].get_port() == servers[j].get_port() && 
                servers[i].get_server_name() == servers[j].get_server_name()) {
                utils::print_error("duplicate server ", "!");
            }
        }
    }
}

void config_file::check_if_ports_is_duplicated(partition_server new_server)
{
    std::vector<std::string> tmp = new_server.get_ports();
    for (size_t i = 1; i < tmp.size(); ++i) 
    {
        if (tmp[i] == tmp[i - 1])
            utils::print_error("duplicate port :" ,tmp[i]);
    }
    if(new_server.get_host() == 1 || new_server.get_ports().size() == 0 || new_server.get_root().empty() || new_server.get_index().empty())
        utils::print_error("you should set : ", " host - port - index - root ");
}

bool config_file::first_char_after_whitespace(const std::string &str, char c)
{
    unsigned long i ;
    for (i = 0; i < str.size(); i++) 
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        break;
    }
    if(str[i] == c)
        return true;
    return false;
}

void config_file::is_valid_host(partition_server *new_server ,const std::string& hostname) {
  addrinfo *res;
  addrinfo hint;
  std::memset(&hint, 0,sizeof(hint));
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  int status = getaddrinfo(hostname.c_str(), nullptr, &hint, &res);
  if (status != 0) {
    freeaddrinfo(res);
    utils::print_error("error in this part :", hostname);
    exit(1);
  }
  sockaddr_in* sock = (sockaddr_in*) res->ai_addr;
  long int ip = htonl(sock->sin_addr.s_addr);
  freeaddrinfo(res);
  new_server->set_host(ip);
}

int config_file::convert_string_to_int(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if(!(str[i] >= 48 && str[i] <= 57))
        {
            utils::print_error("error in this part : ", str);
        }
        i++;
    }
    std::stringstream ss(str);
    int num;
    ss>>num;
    return (num);
}

void config_file::split_by_char_and_store_in_vector(std::string str, char c, std::vector<std::string>& vect)
{
    std::string word;
    std::stringstream ss(str);
    while(!ss.eof())
    {
        std::getline(ss, word, c);
        vect.push_back(word);
    }
}

bool config_file::only_whitespace(const std::string& str) {
  for (unsigned long i = 0; i < str.size(); i++) {
    if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' ) {
      return false;
    }
  }

  return true;
}

int config_file::count_alphabetic_and_check_is_digits(char c, std::string str, int number, int min , int max)
{
    if(only_whitespace(str) || str.empty())
        return(1);
    std::string word;
    for (size_t i = 0; i < str.size() && number > 0; i++)
    {
        if(str[i] == c)
            number--;
    }
    if(number > 0)
        utils::print_error("error in this part : ", str);
    std::stringstream ss(str);
    while(!ss.eof())
    {
        int num;
        getline(ss, word, c);
        num = convert_string_to_int(word);
        if (min >= 0 && max >= 0)
        {
            if(num < min || num > max)
                utils::print_error("error in this part : ", str);
        }
    }
    return(0);  
}

std::vector<partition_server> config_file::get_servers()
{
    return servers;
}

int cal_num_of_server(std::vector<std::string> lines_of_conf)
{
    int num = 0;
    for (std::vector<std::string>::iterator it= lines_of_conf.begin(); it != lines_of_conf.end(); ++it)
    {
        if (*it == "server:")
            num++;
    }
    return (num);
}

int config_file::check_location(std::string index, std::string value, location_param &loc)
{
    if (index == "redirect_URL")
    {
        if(!loc.get_redirect_url().empty())
            utils::print_error("duplicate in this part : ", index);
        value = utils::delete_all_whitespace_and_set_one_space(value);
        std::stringstream ss(value);
        ss>>value;
        if(!ss.eof())
            utils::print_error("error in this part : ", value);
        loc.set_redirect_url(value);
    }
    else if (index == "index")
    {
        value = utils::delete_all_whitespace_and_set_one_space(value);
        if(!loc.get_index().empty())
            utils::print_error("duplicate in this part : ", index);
        std::stringstream ss(value);
        ss>>value;
        if(!ss.eof())
            utils::print_error("error in this part : ", value);
        loc.set_index(value); 
    }
    else if (index == "methods")
    {
        if(loc.get_methods("GET") || loc.get_methods("POST") || loc.get_methods("DELETE"))
            utils::print_error("duplicate in this part : ", index);
        loc.set_methods(value);

    }
    else if (index == "upload_dir")
    {
        if(!loc.get_upload_dir().empty())
            utils::print_error("duplicate in this part : ", index);
        if(!first_char_after_whitespace(value, '/'))
            utils::print_error("error in this part :", value);
        loc.set_upload_dir(value);
    }
    else if (index == "directory_listing")
    {
        value = utils::delete_all_whitespace_and_set_one_space(value);
        if(!loc.get_directory_listing().empty())
            utils::print_error("duplicate in this part : ", index);
        std::stringstream ss(value);
        ss>>value;
        if(!ss.eof() || (value != "true" && value != "false"))
            utils::print_error("error in this part : ", value);
        loc.set_directory_listing(value);
    }
    else if (index == "root")
    {
        if(!loc.get_root().empty())
            utils::print_error("duplicate in this part : ", index);
        if(!first_char_after_whitespace(value, '/') || !loc.get_alias().empty())
            utils::print_error("error in this part :", value);
        loc.set_root(value +'/');
    
    }
    else if (index == "alias")
    {
        if(!loc.get_alias().empty())
            utils::print_error("duplicate in this part : ", index);
        if(!first_char_after_whitespace(value, '/') || !loc.get_root().empty())
            utils::print_error("error in this part :", value);
        loc.set_alias(value);
    
    }else
        return(1);
    return(0);
}

int config_file::cal_how_many_strings_in_string(std::string str, char c)
{
    std::string word;
    std::stringstream ss(str);
    int num_of_port = 0;
    while (!ss.eof())
    {
        std::getline(ss, word, c);
        num_of_port++;
    }
    return num_of_port;
}

void config_file::multiple_srv_with_with_multiple_port(partition_server new_server)
{
    int num_of_server = new_server.get_ports().size();
    for(int i = 0; i < num_of_server ;i++)
    {
        partition_server server;
        server = new_server;
        server.set_port(new_server.get_ports()[i]);
        servers.push_back(server);
    }
    
}

int config_file::check_and_store_data(partition_server *new_server, std::vector<std::string>::iterator& it)
{
    std::stringstream ss(*it);
    std::string index;
    std::string value;
    std::getline(ss, index, ':');
    std::getline(ss, value);

    index.erase(0, index.find_first_not_of(" \t"));
    value.erase(0, value.find_first_not_of(" \t"));
    if (index == "host" && !value.empty())
    {
        value=utils::delete_all_whitespace_and_set_one_space(value);
        if(new_server->get_host() != -1)
            utils::print_error("duplicate in this part : ", index);
        is_valid_host(new_server, value);
    }
    else if (index == "server_name" && !value.empty())
    {
        if(!new_server->get_server_name().empty())
            utils::print_error("duplicate in this part : ", index);
        std::vector<std::string> vect;
        split_by_char_and_store_in_vector(value, ' ', vect);
        new_server->set_server_name(vect);
    }
    else if (index == "port" && !value.empty())
    {
        value = utils::delete_all_whitespace_and_set_one_space(value);
        if(!new_server->get_ports().empty())
            utils::print_error("duplicate in this part : ", index);
        std::vector<std::string> vect;
        count_alphabetic_and_check_is_digits(' ', value, -1, 1, 65536);
        split_by_char_and_store_in_vector(value, ' ', vect);
        new_server->set_ports(vect);
    }
    else if (index == "root" && !value.empty())
    {
        if(!new_server->get_root().empty())
            utils::print_error("duplicate in this part : ", index);
        if(!first_char_after_whitespace(value, '/'))
            utils::print_error("error in this part :", value);
        new_server->set_root(value+'/');
    }
    else if (index == "max_body_size" && !value.empty())
    {
        value = utils::delete_all_whitespace_and_set_one_space(value);
        if(!new_server->get_max_body_size().empty())
            utils::print_error("duplicate in this part : ", index);
        count_alphabetic_and_check_is_digits('\n', value, -1, 0, 21000000);
        new_server->set_max_body_size(value);
    }
    else if (index == "index" && !value.empty())
    {
        value = utils::delete_all_whitespace_and_set_one_space(value);
        if(!new_server->get_index().empty())
            utils::print_error("duplicate in this part : ", index);
        std::stringstream ss(value);
        ss>>value;
        if(!ss.eof())
            utils::print_error("error in this part : ", value);
        new_server->set_index(value);
    }
    else if (index == "error_pages")
    {
        if(value.empty())
        {
            it++;
            std::stringstream se(*it);
            std::getline(se, index, ':');
            std::getline(se, value);
            index.erase(0, index.find_first_not_of(" \t"));
            value.erase(0, value.find_first_not_of(" \t"));
            while(!count_alphabetic_and_check_is_digits('.', index, -1, 100, 599))
            {
                value = utils::delete_all_whitespace_and_set_one_space(value);

                if(!new_server->get_error_pages(index).empty())
                    utils::print_error("duplicate in this part : ", index);
                new_server->set_error_pages(index, value);
                it++;
                if((*it).empty() || only_whitespace(*it))
                    break;
                std::stringstream se(*it);
                std::getline(se, index, ':');
                std::getline(se, value);
                index.erase(0, index.find_first_not_of(" \t"));
                value.erase(0, value.find_first_not_of(" \t"));
                if(index == "location" || index == "host" || index == "server_name" || index =="port" || index == "root" || index == "max_body_size" || index == "index")
                    break;
            }
            it--;
        }
    }
    else if (index == "location")
    { 

        if(!value.empty())
        {
            value = utils::trim_space_from_back(value);
            if(!first_char_after_whitespace(value, '/') || value[value.size()-1] != '/')
                utils::print_error("error in this part :", value);
            std::string save;
            save = value;
            it++;
            std::stringstream sl(*it);
            std::getline(sl, index, ':');
            std::getline(sl, value);
            index.erase(0, index.find_first_not_of(" \t"));
            value.erase(0, value.find_first_not_of(" \t"));
            location_param loc;
            while (index == "redirect_URL" || index == "index" || index == "methods" || index == "directory_listing" || index == "upload_dir" || index == "directory_listing" || index == "root" || index == "alias")
            {
                if(check_location(index, value, loc))
                    utils::print_error("error in this part: ", index);      
                it++;
                if (((*it).empty() || only_whitespace(*it)))
                    break;      
                std::stringstream sn(*it);
                std::getline(sn, index, ':');
                std::getline(sn, value);
                index.erase(0, index.find_first_not_of(" \t"));
                value.erase(0, value.find_first_not_of(" \t"));
            }
            it--;
            if(loc.get_methods("GET") != true && loc.get_methods("POST") != true && loc.get_methods("DELETE") != true)
                loc.set_methods("GET");
            new_server->set_location(save, loc);

        }
    }
    else if(!only_whitespace(index))
        utils::print_error("error in this part :", index);    
    return(0);
}

std::vector<partition_server> config_file::split_servers(std::vector<std::string> lines_of_conf)
{
    int num_of_server = cal_num_of_server(lines_of_conf);
    if (num_of_server > 0)
    {
        for (std::vector<std::string>::iterator it= lines_of_conf.begin(); it != lines_of_conf.end(); ++it)
        {
            if (*it == "server:")
            {
                partition_server new_server;
                it++;
                while (it != lines_of_conf.end() && (it + 1) != lines_of_conf.end() && *(it + 1) != "server:")
                {
                    check_and_store_data(&new_server, it);
                    ++it;
                    if ((it + 1) != lines_of_conf.end() && (*(it + 1) == "server:") && !only_whitespace(*it))
                        utils::print_error("error in this part :", *it);
                }
                check_if_ports_is_duplicated(new_server);
                if (new_server.get_max_body_size().empty())
                    new_server.set_max_body_size("100000");
                multiple_srv_with_with_multiple_port(new_server);
                if(it == lines_of_conf.end())
                    return servers;
            }
        }
    }
    else
    {
        std::cout<<"error in this part "<<std::endl;
    }
    return servers;
}

config_file::config_file(const std::string& name_of_file)
{
    std::string line;
    std::ifstream file(name_of_file);
    if(!file.is_open())
    {
        throw std::runtime_error("file doesn't exist");
    }
    while (std::getline(file, line))
        lines_of_conf.push_back(line);
    lines_of_conf.push_back("\n");
    split_servers(lines_of_conf);
    check_if_duplicate_server();
}

bool config_file::delete_file(std::string name_of_file)
{
    std::ifstream file(name_of_file);
    if(file)
    {
        file.close();
        if (std::remove(name_of_file.c_str()) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

config_file::config_file()
{
    
}

config_file::~config_file()
{
}
