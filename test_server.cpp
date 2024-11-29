#include <iostream>
#include <string>
#include <curl/curl.h>

// Helper function to capture response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to test the HTTP server's GET /compute endpoint
bool testComputeEndpoint() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    std::string url = "http://localhost:8081/compute";
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // Validate the response
    try {
        int elapsedTime = std::stoi(response);
        if (elapsedTime < 1000 || elapsedTime > 20000) {
            std::cerr << "Elapsed time out of range: " << elapsedTime << "ms" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "Failed to parse response: " << response << std::endl;
        return false;
    }

    return true;
}

int main() {
    std::cout << "Running tests..." << std::endl;

    bool computeTestResult = testComputeEndpoint();

    if (computeTestResult) {
        std::cout << "Test for /compute endpoint PASSED." << std::endl;
    } else {
        std::cerr << "Test for /compute endpoint FAILED." << std::endl;
        return 1;
    }

    return 0;
}

