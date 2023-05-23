#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include <tuple>
#include "functions.h"

using namespace std;




// Find the value of an attribute in an HTML tag
void errorbased(string url)
{
    cout <<"Performing error based SQL injection...\n\n";
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize curl\n";
    }

    // URL to send requests to
    //std::string url = "http://localhost/tigernetBD-master/tgnet/login.php";

    // Open the file containing payloads
    std::ifstream infile("payloads.txt");
    if (!infile.is_open())
    {
        std::cerr << "Failed to open payloads file\n";
        curl_easy_cleanup(curl);
        //return 1;
    }

    // Store the payloads in a vector
    std::vector<std::string> payloads;
    std::string payload;
    while (std::getline(infile, payload))
    {
        payloads.push_back(payload);
        std::cout << payload<<"\n";
    }

    // Close the file
    infile.close();

    std::string errrormessage="You have an error in your SQL syntax; check the manual that corresponds to your";
    bool vulnerable=false;

    std::string response;
    //std::string url = "http://localhost/DVWA-master/vulnerabilities/sqli/?id=" +payload+"&Submit=Submit";



    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "Failed to perform curl request: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        //return 1;
    }

    if (response.empty())
    {
        std::cerr << "Empty response data\n";
        //continue;
    }


    // Send requests for each payload in the vector
    for (const auto  payload : payloads)
    {

        std::cout <<"Used payload : "<<payload<<"\n......................\n";

        std::vector<std::tuple<std::string, std::string, std::string>> params = extract_parameters(response);

        // Print the parameter names
        string url1=url;
        url1+="/?";
        int cnt=1;
        for (const auto& param : params)
        {
            // std::cout <<"Enter the value of " <<get<0>(param) << "\n";
            std::string para="";
            if(cnt==1)
            {
                url1+=get<0>(param)+"="+payload;
                cnt++;
            }
            else if(std::get<1>(param)=="submit"&&std::get<0>(param).length()!=0&&std::get<2>(param).length()!=0)
            {
                url1+="&"+get<0>(param)+"="+get<2>(param);
                cnt++;
            }
            else if(std::get<1>(param)=="submit"&&std::get<0>(param).length()!=0&&std::get<2>(param).length()==0)
            {
                url1+="&"+get<0>(param)+"="+"Submit";
                cnt++;
            }
            else
            {
                url1+="&"+get<0>(param)+"="+para;
                cnt++;
            }
        }
        std::cout << url1 << "\n";

        std::string response1;
        //std::string url = "http://localhost/DVWA-master/vulnerabilities/sqli/?id=" +payload+"&Submit=Submit";



        curl_easy_setopt(curl, CURLOPT_URL, url1.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response1);
        //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "Failed to perform curl request: " << curl_easy_strerror(res) << "\n";
            curl_easy_cleanup(curl);
            //return 1;
        }

        if (response1.empty())
        {
            std::cerr << "Empty response data\n";
            //continue;
        }




        //std::cout << "Response: " << response1 << "\n";
        if(Find(response1,errrormessage)!=std::string::npos)
        {
            vulnerable=true;
        }

    }

    curl_easy_cleanup(curl);
    if(vulnerable)
    {
        std::cout << "The website is vulnerable to errorbased sql injection\n\n";
    }
    else std::cout << "The website is not vulnerable to errorbased sql injection\n\n";
    //return 0;
}
