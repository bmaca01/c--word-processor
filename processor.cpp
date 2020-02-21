#include<iostream>
#include<string>
#include<cctype>
#include<sstream>
#include<fstream>
/*
 *  'word' - sequence of non-whitespace characters
 *  'empty line' - line with no characters at all
 *  'blank line' - line containing only one or more whitespace characters
 *  'line length' - number of characters in each line
 *  'paragraph' - a collection of lines, no blank lines or empty lines
 *  'indent' - spaces at the beginning of the first line of a paragraph
 *
 *  objective: reads lines of input and prints out paragraphs
 */
using namespace std;

namespace arg_info {
    bool is_file=0, is_flag=0, is_valid=0, is_LL=0, is_IN=0, has_equal=0;
    int flag_val=0; 
    int file_cnt=0;

    void verify_arg(string arg);
    void reset_state();

    void verify_arg(string arg) {
        if (arg[0] == '-') {
            string flag_arg = arg.substr(0, 3);
            is_flag = 1;
            is_LL = (flag_arg.compare("-LL") == 0) ? true : false;
            is_IN = (flag_arg.compare("-IN") == 0) ? true : false;
            is_valid = is_LL || is_IN;    
            has_equal = arg[3] == '=';
            if (is_valid && has_equal) {
                string val = arg.substr(4);
                try {
                    flag_val = stoi(val);
                    for (const char c : val) {
                        if (!(isdigit(c)))
                            flag_val = 0;
                    }
                }
                catch (exception &e) {
                    flag_val = 0;
                }
            }
        }
        else {
            file_cnt++;
            is_file = 1;
        }
    }
    void reset_state() {
        is_file=0, is_flag=0, is_valid=0, is_LL=0, is_IN=0, has_equal=0;
        flag_val=0; 
    }
    void print_state() {
        cout << "\tIs file?: " << is_file << endl;
        cout << "\tIs flag?: " << is_flag << endl;
        cout << "\tIs valid?: " << is_valid << endl;
        cout << "\tIs LL?: " << is_LL << endl;
        cout << "\tIs IN?: " << is_IN << endl;
        cout << "\tHas equal?: " << has_equal << endl;
        cout << "\tFlag val: " << flag_val << endl;
        cout << "\tFile count: " << file_cnt << endl;
    }
}

int main(int argc, char* argv[]) {
    using namespace arg_info;
    int line_len = 40;
    int indent = 8;
    ifstream file;
    istream* in = &cin;
    cout << argc << endl;

    for (int i=0; i < argc; i++) {
        string curr_arg(argv[i]);
        verify_arg(curr_arg);         
        cout << "current arg: " << curr_arg << endl;

        if (is_flag) {
            if (!has_equal && is_valid) {
                cout << "MISSING = SIGN " << curr_arg << endl;
                return 1;
            }
            else if (!is_valid) {
                cout << "UNRECOGNIZED FLAG " << curr_arg << endl;
                return 1;
            }
            else if (has_equal && is_valid) {
                if (flag_val == 0){
                    cout << "VALUE NOT INTEGER > 0 " << curr_arg << endl;
                    return 1;
                }
                else if (is_LL)
                    line_len = flag_val;
                else if (is_IN)
                    indent = flag_val;
            }
        }
        else if (is_file && i > 0) {
            if (file_cnt > 2) {
                cout << "TOO MANY FILENAMES" << endl;
                return 1;
            } 
            else {
                file.open(curr_arg);
                if (file.is_open() == false) {
                    cout << "CANNOT OPEN " << curr_arg << endl;
                    return 1;
                }
            }
        }
        reset_state();
    }

    cout << "line len: " << line_len << endl;
    cout << "indent: " << indent << endl;
    file.close();
    print_state();
    return 0;
}


