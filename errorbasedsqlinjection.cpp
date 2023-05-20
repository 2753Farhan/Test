#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include "functions.h"

// Callback function to store response data

// Compute the prefix function for the pattern


// Search for the pattern within the text using the KMP algorithm


void errorbased() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
       return ;
    }

    // URL to send requests to
    //std::string url = "http://localhost/tigernetBD-master/tgnet/login.php";

    // Open the file containing payloads
    std::ifstream infile("payloads.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open payloads file\n";
        curl_easy_cleanup(curl);
//        return 1;
    }

    // Store the payloads in a vector
    std::vector<std::string> payloads;
    std::string payload;
    while (std::getline(infile, payload)) {
        payloads.push_back(payload);
        std::cout << payload<<"\n";
    }

    // Close the file
    infile.close();

    std::string errrormessage="You have an error in your SQL syntax; check the manual that corresponds to your";
    bool vulnerable=false;

    // Send requests for each payload in the vector
    for (const auto  payload : payloads) {

        std::cout <<"Used payload : "<<payload<<"\n......................\n";
        std::string response;
        std::string url = "http://localhost/DVWA-master/vulnerabilities/sqli/?id=" +payload+"&Submit=Submit";


        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to perform curl request: " << curl_easy_strerror(res) << "\n";
            curl_easy_cleanup(curl);
           return ;
        }

        if (response.empty()) {
            std::cerr << "Empty response data\n";
            continue;
        }

        std::cout << "Response: " << response << "\n";
        if(Find(response,errrormessage)!=std::string::npos){
            vulnerable=true;
        }

    }

    curl_easy_cleanup(curl);
    if(vulnerable){
        std::cout << "The website is vulnerable to error based sql injection\n";
    }
    else std::cout << "The website is not vulnerable to error based sql injection\n";
 //   return 0;
}
