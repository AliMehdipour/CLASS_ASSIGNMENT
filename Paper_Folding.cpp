//DEVELOPER: ALI MEHDIPOUR 
//USING BACKTRACKING WITH PROMISING METHOD

#include <iostream>
#include <string>
#include <stack>
#include <fstream>

using namespace std;
std::ifstream infile("Input.txt");

//OBJECTS(DYNAMIC 2D-ARRAY WITH COPY CONSTRUCTOR-OVELOADED FUNCTIONS & ...
    class Matrix {
public:
    string** matrix = 0;
    int rows = 0;
    int columns = 0;
    Matrix(int N, int M) {
        if (N <= 8 && M <= 4 && (M * N) <= 16) {
            rows = N;
            columns = M;
            matrix = new  string * [N];
            for (int i = 0; i < N; i++) {
                string* my_array2 = nullptr;
                my_array2 = new string[M];
                matrix[i] = my_array2;
            }
           
        }
    }
    //copy constructor
    Matrix(const Matrix& tmp1) {
        this->rows = tmp1.rows;
        this->columns = tmp1.columns;
        matrix = new string * [rows];
        for (int i = 0; i < rows; i++) {
            string* my_array2 = nullptr;
            my_array2 = new string[columns];
            matrix[i] = my_array2;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                this->matrix[i][j] = tmp1.matrix[i][j];
            }
        }
    }
    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[]matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
        rows = 0;
        columns = 0;
    }
    //Assignment operator
    Matrix& operator =(const Matrix& tmp) {
        this->rows = tmp.rows;
        this->columns = tmp.columns;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                this->matrix[i][j] = tmp.matrix[i][j];
            }
        }
        return *this;
    }
    void Print(void) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                std::cout << matrix[i][j] << "--";
            }
            cout << "\n";
        }
        cout << "\n================================================\n";
    }
    void Set_Alphabet(void){
        int count = 0;
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                this->matrix[i][j] = char(65 + count);
                    count ++;
            }

    }
    }
};

//GLOBAL VARIABLES
    string STRING;
    stack<string> PROCESS;
//METHODS 
    bool Promising(Matrix& paper);
    bool sub_str_check(string& s1);
    string reverse_str(string& str);
    bool Folding(Matrix& mat);
    Matrix VIL(const Matrix& mat, int column_number);
    Matrix VIR(const Matrix& mat, int column_number);
    Matrix HIU(const Matrix& mat, int column_number);
    Matrix HIL(const Matrix& mat, int column_number);

int main()
{
    //READ FILE : 
    int read_n = 0;
    int read_m = 0;
    string read_str = " ";
    string line;
    ifstream myfile;
    myfile.open("Input.txt");

    if (!myfile.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    if (getline(myfile, line))
        read_n = stoi(line);
    if (getline(myfile, line))
        read_m = stoi(line);
    if (getline(myfile, line))
        read_str = line;
    //END READ
    Matrix s(read_n, read_m);
    s.Set_Alphabet();
    STRING = read_str; 
    if (Folding(s))
        while (!PROCESS.empty()) {
            cout << PROCESS.top()<<" ";
            PROCESS.pop();
        }
    cout << '\n';

    /* TEST CASES:
    Matrix s(4, 4);
    Matrix s(3,4);
    Matrix s(2, 3);
    s.Set_Alphabet();
    s.Print();
    STRING = "ONBCDAMPLIEHGFJK";
    STRING = "HDLFEABIJKCG";
    STRING = "FCBADE";*/

}

bool Promising(Matrix& paper) {
    for (int i = 0; i < paper.rows; i++) {
        for (int j = 0; j < paper.columns; j++) {
            if(paper.matrix[i][j].length()>=3){
                string temp = reverse_str(paper.matrix[i][j]);
                if(sub_str_check(paper.matrix[i][j])||sub_str_check(temp)){/*Do Nothing*/ }
                else {
                    //std::cout << "NOT PROMISED\n";
                    return false;
                }
            }
     
        }
    }
    return true;
}
bool sub_str_check(string& s1){
    //Check every Consistent charcacter between 2 other chars that cannot change this formation with folding paper
    //Each s1 has s1.length()-2 constant ABC that B is fixed between A & B
    for (int i = 0; i < s1.length() - 2;i++) {
        if (STRING.find(s1.substr(i,i + 2)) != std::string::npos) {
            return true;
        }
       
    }
    return false;
}
string reverse_str(string& str){
    int n = str.length();
    //Temporary string to store the reverse
    string rev;
    for (int i = n - 1; i >= 0; i--)
        rev.push_back(str[i]);
    return rev;
}
bool Folding(Matrix& mat) {
    if (mat.matrix[0][0] == STRING) {
        return true;
    }
    else {
        if (Promising(mat)) {
            /*for (each possible child of father){
                if(Folding(child))
                return true;
                }
                ALL POSSIBLE CHILD=2X(N-1)+2X(M-1): 2(N-1) FOR HORIZENTALS((N-1)FOR HIU AND (N-1)FOR HIL) AND
                2(M-1) FOR VERTICALS((M-1)FOR VIR AND (M-1)FOR VIL)
            */
            //M-1 POSSIBLE VIL'S
            for (int i = 1; i <= mat.columns - 1; i++) {
                Matrix child= VIL(mat,i);
                if (Folding(child))
                {
                    string temp = "VIL";
                    temp[1] = i+'0';
                    PROCESS.push(temp);
                    return true;
                }
                    
            }
            //M-1 POSSIBLE VIR'S
            for (int i = 1; i <= mat.columns - 1; i++) {
                Matrix child = VIR(mat, i);                

                if (Folding(child))
                {
                    string temp = "VIR";
                    temp[1] = i + '0';
                    PROCESS.push(temp);
                    return true;
                }
            }
            //N-1 POSSIBLE HIU'S
            for (int i = 1; i <= mat.rows - 1; i++) {
                Matrix child = HIU(mat, i);                

                if (Folding(child))
                {
                    string temp = "HIU";
                    temp[1] = i + '0';
                    PROCESS.push(temp);
                    return true;
                }
            }
            //N-1 POSSIBLE HIL'S
            for (int i = 1; i <= mat.rows - 1; i++) {
                Matrix child = HIL(mat, i);                

                if (Folding(child))
                {
                    string temp = "HIL";
                    temp[1] = i + '0';
                    PROCESS.push(temp);
                    return true;
                }
            }
            return false;
        }
        else {
            return false;
        }
    }
}
Matrix VIL(const Matrix& mat, int H_line_number){
    //WHAT IS THE SIZE OF NEW MATRIX ? MAX(column_number,mat.columns-column_number)
    int new_columns_count = max(H_line_number, mat.columns - H_line_number);
    Matrix new_mat(mat.rows, new_columns_count);
    int rev_count = 0;
    for (int i = H_line_number; i >=1 ;i--) {
        for (int j = 0; j < mat.rows; j++) {
            string rev = reverse_str(mat.matrix[j][i-1]);
            string final_str;
            if (i + (2 * (rev_count)+1) <= mat.columns)
                final_str = rev + mat.matrix[j][i + (2 * (rev_count)+1) - 1];
            else
                final_str = rev;
            new_mat.matrix[j][rev_count] = final_str;
        }
        rev_count++;
    }
    //Not Changed
    if ((mat.columns - H_line_number) > H_line_number) {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = rev_count + H_line_number; j < mat.columns; j++) {
                new_mat.matrix[i][j-1] = mat.matrix[i][j];
            }
        }
    }
    return new_mat;
}
Matrix VIR(const Matrix& mat, int H_line_number) {
    //WHAT IS THE SIZE OF NEW MATRIX ? MAX(column_number,mat.columns-column_number)
    int new_columns_count = max(H_line_number, mat.columns - H_line_number);
    Matrix new_mat(mat.rows, new_columns_count);
    int rev_count = 0;
    for (int i = H_line_number; i <mat.columns; i++) {
        for (int j = 0; j < mat.rows; j++) {
            string rev = reverse_str(mat.matrix[j][i]);
            string final_str;
            if (i - (2 * (rev_count)+1)>=0)
                final_str = rev+ mat.matrix[j][i - (2 * (rev_count)+1)] ;
            else
                final_str = rev;
            new_mat.matrix[j][new_mat.columns-rev_count-1] = final_str;
        }
        rev_count++;
    }
    //Not Changed
    if ((mat.columns-H_line_number)<H_line_number) {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = H_line_number-rev_count; j >= 1; j--) {
                new_mat.matrix[i][j - 1] = mat.matrix[i][j];
            }
        }
    }
    return new_mat;
}
Matrix HIU(const Matrix& mat, int Line_number){
    //WHAT IS THE SIZE OF NEW MATRIX ? MAX(column_number,mat.columns-column_number)
    int new_rows_count = max(Line_number, mat.rows - Line_number);
    Matrix new_mat(new_rows_count,mat.columns);
    int rev_count = 0;
    for (int i = Line_number; i >= 1; i--) {
        for (int j = 0; j < mat.columns; j++) {
            string rev = reverse_str(mat.matrix[i-1][j]);
            string final_str;
            if (i + (2 * (rev_count)+1) <= mat.rows)
                final_str = rev + mat.matrix[i + (2 * (rev_count)+1) - 1][j];
            else
                final_str = rev;
            new_mat.matrix[rev_count][j] = final_str;
        }
        rev_count++;
    }
    //Not Changed
    if ((mat.rows - Line_number) > Line_number) {
        for (int i = 0; i < mat.columns; i++) {
            for (int j = rev_count + Line_number; j < mat.rows; j++) {
                new_mat.matrix[j-1][i] = mat.matrix[j][i];
            }
        }
    }
    return new_mat;
}
Matrix HIL(const Matrix& mat, int Line_number){
    //WHAT IS THE SIZE OF NEW MATRIX ? MAX(column_number,mat.columns-column_number)
    int new_rows_count = max(Line_number, mat.rows - Line_number);
    Matrix new_mat(new_rows_count,mat.columns);
    int rev_count = 0;
    for (int i = Line_number; i < mat.rows; i++) {
        for (int j = 0; j < mat.columns; j++) {
            string rev = reverse_str(mat.matrix[i][j]);
            string final_str;
            if (i - (2 * (rev_count)+1) >= 0)
                final_str = rev + mat.matrix[(i)-(2 * (rev_count)+1)][j];
            else
                final_str = rev;
            new_mat.matrix[new_mat.rows - rev_count - 1][j] = final_str;
        }
        rev_count++;
    }
    //Not Changed
    if ((mat.rows - Line_number) < Line_number) {
        for (int i = 0; i < mat.columns; i++) {
            for (int j = Line_number - rev_count; j >= 1; j--) {
                new_mat.matrix[j-1][i] = mat.matrix[j][i];
            }
        }
    }
    return new_mat;
}

