#include "Server.hpp"
#include <thread>
#define PATH "/Users/rarraji/Desktop/prj/WebServMerge/pages"

void Server::checkResponse(int socket, std::string host , std::string port)
{
    bool check_red = false;
    std::cout << "host: " << host  << std::endl;
    std::cout << "port: " << port  << std::endl;
    std::cout << "Socket: " << socket  << std::endl;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        // check man server 
        if(this->servers[i].get_host().compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
        {
            std::cout << "url : " << mapinfo[socket].request.response.url << std::endl;
            std::cout << "root + url : " <<  this->servers[i].get_root() + mapinfo[socket].request.response.url << std::endl;

            //check is url is location or not
            if(this->servers[i].get_location_first(mapinfo[socket].request.response.url).compare(mapinfo[socket].request.response.url) == 0) 
            {
              check_red = true;
              std::cout << "DONE IS LOCATION" << std::endl;
            }
            else
              std::cout << "ISN'T LOCATION" << std::endl;
            //check url is valide in location;
        }
        // check is dir or file 
        struct stat fileStat;
        std::string tmp1;
        if (mapinfo[socket].request.response.url.find(this->servers[i].get_root()) == std::string::npos) // cheak url isgisila root
            tmp1 = this->servers[i].get_root() + mapinfo[socket].request.response.url;
        else
            tmp1 = mapinfo[socket].request.response.url;

        if (stat(tmp1.c_str(), &fileStat) == 0) // checkdir
        {
            int i = 0;
            if (S_ISDIR(fileStat.st_mode))
            {
                std::cout << "dossier\n";
                std::string tmp  = tmp1;
                std::cout << "tmp : " << tmp  << std::endl;

                //check_if_location_end "/"
                if (tmp[tmp.length() - 1] == '/' && check_red == true)
                    std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
                if (tmp[tmp.length() - 1] != '/')
                {
                    mapinfo[socket].request.response.url += '/';
                    if(check_red == true)
                      std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
                    mapinfo[socket].request.response.redur = true;
                    mapinfo[socket].request.response.new_redur = mapinfo[socket].request.response.url;
                    std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
                    i = 1;
                    
                }
                //check red url 
                else if (this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url().length() >= 1)
                {
                    std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url() << std::endl;
                    mapinfo[socket].request.response.redur = true;
                    mapinfo[socket].request.response.new_redur = this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url();
                    std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
                    i = 1;
                    //check_redirect_URL
                }
                else
                    mapinfo[i].request.response.redur = false;
                // for (std::map<std::string, bool>::iterator it = this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.begin(); it != this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.end(); ++it)
                // {
                //     //check methodes
                // }
                if (i == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_index().length() > 1)
                {
                //    mapinfo[socket].request.response.root = this->servers[i].get_root() + mapinfo[socket].request.url;
                   mapinfo[socket].request.response.url = this->servers[i].get_root() + this->servers[i].get_location_first(mapinfo[socket].request.url) + this->servers[i].get_location(mapinfo[socket].request.url).get_index();
                    // this->servers[i].get_location(mapinfo[socket].request.url)
                    std::cout << "index To serve : " << mapinfo[socket].request.response.url << std::endl;
                    i = 1;
                }
                std::cout << "directory_listing : " << this->servers[i].get_location(mapinfo[socket].request.url).get_directory_listing() << std::endl;
                if (i == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_directory_listing().compare("true"))
                {
                  mapinfo[socket].request.response.directory_listing = true;
                }
                break;
            }
            else
            {
                mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.url;
                std::cout << "FILE : " << mapinfo[socket].request.response.url << std::endl;
                std::cout << "File Done" << std::endl;    
            }
        }
        
    }
    // exit(1);
}