#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

//slices a vector, it's templated so that I may copy this for future needs
template <typename T>
std::vector<T> slice_vec(const std::vector<T> &vec, int start, int end){
    if(end < start) return std::vector<T>();
    start = std::max(0, start);
    end = std::min((int)vec.size(), end);
    std::vector<T> res(end - start);
    copy(vec.begin() + start, vec.begin() + end, res.begin());
    return res;
}

const std::array<std::string, 6> supportedOperations = {"+", "-", "*", "/", "^","x"};

//determines if the c char is an operation character
bool testOperation(std::string c){
    for(const auto t : supportedOperations){
        if(t == c) return true;
    }
    return false;
}

//lv= left value, op= operation, rv= right value
//simply computes one basic operation
float simple_compute(std::string lv, std::string op, std::string rv){
    float nlv = std::stof(lv);
    float nrv = std::stof(rv);
    if(op == "+") return nlv + nrv;
    if(op == "-") return nlv - nrv;
    if(op == "*" || op == "x") return nlv * nrv;
    if(op == "/") return nlv / nrv;
    if(op == "^") return std::pow(nlv, nrv);
    
    throw std::runtime_error("invalid operation: " + op);
}

//main compute function, it accepts a string to compute and it parses it.
//it will also call itself recursively to compute subcomputetions defined with () brakets.
float compute(std::string computation){
    //in segments_vec I will add the components of of the computations.
    //components can only be numbers or operations, 
    //brakets will be pre-computed into a number before pushing to the vector
    std::vector<std::string> segments_vec;

    std::string numeric_cursor = "";
    std::string subcomputation = "";

    //depth value is used to track brakets levels
    unsigned short depth = 0;

    bool toNegate = false;
    
    for(char c : computation){
        if(c == ' ') continue;
        //if opening braket, then increase depth
        if(c == '(') {
            //if no longer operating on depth 0, then add the braket to the subcomputation
            if(depth > 0) subcomputation += c;
            depth++;
            continue;
        }
        if(c == ')') {
            depth--;
            //if we reached back depth 0, compute subcomputation
            if(depth == 0) {
                const auto res = compute(subcomputation);
                segments_vec.push_back(std::to_string(res * (toNegate ? -1 : 1)));
                toNegate = false;
                subcomputation.clear();
            }
            //else add closing braket to subcomputation.
            else subcomputation += c;
            continue;
        }
        if(depth > 0) { 
            //if not operating on depth 0, then just add characters to the subcomputation stirng
            subcomputation += c;
            continue;
        }
        //if character is numeric or . then add it to the numeric_cursor
        //nothing will happen if a _ or other characters are in the middle, so 1_000 and 1,000 are valid numbers
        if((c >= '0' && c <= '9') || c == '.') numeric_cursor += c;
        else if(numeric_cursor.length()) {
            const auto insertion = std::stof(numeric_cursor) * (toNegate ? -1 : 1);
            segments_vec.push_back(std::to_string(insertion));
            toNegate = false;
            numeric_cursor.clear();
        }
        //if a - symbol is encountered, and the last segment added was another operation...
        //then I assume that user wants to negate the following value in the computation.
        if(c == '-' && testOperation(segments_vec.back())){
            toNegate = true;
            
        }
        //adding computation symbol to the vec
        else if(testOperation(std::string() + c)){
            segments_vec.push_back(std::string() + c);
        }
    }
    //if numeric_cursor is not empty, at the end, I push it to the array
    if(numeric_cursor.length()) {
        const auto insertion = std::stof(numeric_cursor) * (toNegate ? -1 : 1);
        
        segments_vec.push_back(std::to_string(insertion));
    }

    //this lambda function finds the first operation to compute
    //pow operation has prio, multiplication and division have secondary prio
    //if there is not pow, multiplication or division, I simply return 1
    //1 assumes that the first operation to compute is at idx 1
    const auto searchNextPivot = [](const std::vector<std::string> &vec){
        for(int i = 1; i < vec.size(); i += 2){
            if(vec[i] == "^") return i;
        }

        for(int i = 1; i < vec.size(); i += 2){
            if(vec[i] == "*" || vec[i] == "/" || vec[i] == "x") return i;
        }
        return 1;
    };

    //until my segments_vec is not empty, find the prio computation and compute itself
    while(segments_vec.size() > 1){
        const int pivot = searchNextPivot(segments_vec);

        const auto head = slice_vec(segments_vec, 0, pivot - 1);
        const auto tail = slice_vec(segments_vec, pivot + 2, segments_vec.size());
        const auto needle = std::to_string(
            simple_compute(segments_vec[pivot - 1], segments_vec[pivot], segments_vec[pivot + 1])
        );

        //replacing the 3 segments involved in the last computation with the result of it.
        segments_vec.clear();
        segments_vec.insert(segments_vec.end(), head.begin(), head.end());
        segments_vec.push_back(needle);
        segments_vec.insert(segments_vec.end(), tail.begin(), tail.end());
    }

    return std::stof(segments_vec[0]);
}

int main(int argc, char* argv[]){
    // so proud! :D
    std::string computation = argv[1];
    std::cout << computation << " = " << compute(computation) << std::endl;
}
