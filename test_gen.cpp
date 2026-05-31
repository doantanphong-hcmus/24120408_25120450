/*
  test_gen.exe int 1 
  test_gen.exe int 2 
  test_gen.exe int 3
  test_gen.exe int 4 
  test_gen.exe int 5 
  test_gen.exe strlexi 1 
  test_gen.exe strlexi 2 
  test_gen.exe strlexi 3 
  test_gen.exe strlexi 4 
  test_gen.exe strlexi 5  
  test_gen.exe strlenlexi 1 
  test_gen.exe strlenlexi 2 
  test_gen.exe strlenlexi 3 
  test_gen.exe strlenlexi 4 
  test_gen.exe strlenlexi 5 
 */

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// ==========================================
// MODULE 1: INTERGER SORT (int)
// ==========================================
void generate_int_test(int test_id) {
    int n = 100000; // Giới hạn N = 10^5 cho bài A
    cout << n << "\n";
    
    // Khởi tạo bộ sinh số ngẫu nhiên chuẩn 
    random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<long long> dist(-2147483648LL, 2147483647LL);

    switch (test_id) {
        case 1: {
            // Test 1: Cực hạn giá trị âm 
            // Trộn ngẫu nhiên lượng lớn số -2147483648, 2147483647 và 0
            vector<int> a;
            a.reserve(n);
            for(int i = 0; i < 40000; ++i) a.push_back(-2147483648LL);
            for(int i = 0; i < 30000; ++i) a.push_back(2147483647LL);
            for(int i = 0; i < 30000; ++i) a.push_back(0);
            
            shuffle(a.begin(), a.end(), gen);
            for(int x : a) cout << x << "\n";
            break;
        }
        case 2: {
            // Test 2: Mảng toàn số 0
            for(int i = 0; i < n; ++i) cout << 0 << "\n";
            break;
        }
        case 3: {
            // Test 3: Chênh lệch bit lớn (Alternating Min/Max)
            // Lặp lại chuỗi [MAX, MIN, 1, -1] để phá bucket
            int pattern[] = {2147483647, -2147483648, 1, -1};
            for(int i = 0; i < n; ++i) {
                cout << pattern[i % 4] << "\n";
            }
            break;
        }
        case 4: {
            // Test 4: Mảng sắp xếp ngược và có phần tử trùng lặp
            // Đi lùi từ 50000 xuống, mỗi số xuất hiện 2 lần
            int val = 50000;
            for(int i = 0; i < n / 2; ++i) {
                cout << val << "\n" << val << "\n";
                val--;
            }
            break;
        }
        case 5: {
            // Test 5: Tối đa số lượng phần tử với giá trị ngẫu nhiên hoàn toàn
            for(int i = 0; i < n; ++i) {
                cout << dist(gen) << "\n";
            }
            break;
        }
        default:
            cerr << "Error: Invalid test_id for 'int' (Must be 1-5)\n";
    }
}

// ==========================================
// MODULE 2: LEXICOGRAPHIC SORT (strlexi)
// ==========================================
void generate_strlexi_test(int test_id) {
    int n = 100000; // Giới hạn N = 10^5 cho bài B
    cout << n << "\n";

    random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<> char_dist(0, 25); // random từ 'a' đến 'z'

    // Hàm lambda hỗ trợ sinh chuỗi ngẫu nhiên
    auto random_string = [&](int len) {
        string s;
        s.reserve(len);
        for(int i = 0; i < len; ++i) {
            s += (char)('a' + char_dist(gen));
        }
        return s;
    };

    switch (test_id) {
        case 1: {
            // Test 1: Tất cả chuỗi giống hệt nhau (Độ dài max 100)
            string s(100, 'a');
            for (int i = 0; i < n; ++i) {
                cout << s << "\n";
            }
            break;
        }
        case 2: {
            // Test 2: Mảng đã được sắp xếp sẵn (Best-case)
            vector<string> data;
            data.reserve(n);
            for (int i = 0; i < n; ++i) {
                data.push_back(random_string(100));
            }
            sort(data.begin(), data.end());
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 3: {
            // Test 3: Mảng sắp xếp ngược hoàn toàn (Worst-case gán mảng)
            vector<string> data;
            data.reserve(n);
            for (int i = 0; i < n; ++i) {
                data.push_back(random_string(100));
            }
            sort(data.rbegin(), data.rend());
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 4: {
            // Test 4: Kẻ hủy diệt Insertion Sort (Ngưỡng N < 32 của msd_sort)
            int chunk_size = 31;
            vector<string> data;
            data.reserve(n);
            
            for (int i = 0; i < n; i += chunk_size) {
                string prefix = random_string(90); // 90 ký tự đầu giống hệt
                vector<string> suffixes;
                
                int current_chunk = min(chunk_size, n - i);
                for (int j = 0; j < current_chunk; ++j) {
                    suffixes.push_back(random_string(10));
                }
                // Sắp xếp ngược 10 ký tự cuối để ép Insertion Sort chạy O(N^2)
                sort(suffixes.rbegin(), suffixes.rend()); 
                
                for (const string& suf : suffixes) {
                    data.push_back(prefix + suf);
                }
            }
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 5: {
            // Test 5: Tiền tố chung cực dài (Phá vỡ tính hiệu quả của Radix Sort)
            vector<string> data;
            data.reserve(n);
            string prefix(99, 'a');
            for (int i = 0; i < n; ++i) {
                // 99 ký tự 'a', ký tự cuối ngẫu nhiên
                data.push_back(prefix + (char)('a' + char_dist(gen)));
            }
            shuffle(data.begin(), data.end(), gen);
            for (const string& s : data) cout << s << "\n";
            break;
        }
        default:
            cerr << "Error: Invalid test_id for 'strlexi' (Must be 1-5)\n";
    }
}

// ==========================================
// MODULE 3: LENGTH-AWARE SORT (strlenlexi)
// ==========================================
void generate_strlenlexi_test(int test_id) {
    int n = 10000; 
    cout << n << "\n";
    random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<> char_dist(0, 25); 
    auto random_string = [&](int len) {
        string s;
        s.reserve(len);
        for(int i = 0; i < len; ++i) {
            s += (char)('a' + char_dist(gen));
        }
        return s;
    };

    switch (test_id) {
        case 1: {
            // Test 1: Mảng trùng lặp cực đại (10000 chuỗi toàn 'a' dài 100)
            string s(100, 'a');
            for (int i = 0; i < n; ++i) {
                cout << s << "\n";
            }
            break;
        }
        case 2: {
            // Test 2: Insertion Sort Killer (Ngưỡng N < 25)
            int chunk_size = 24;
            vector<string> data;
            data.reserve(n);
            
            for (int i = 0; i < n; i += chunk_size) {
                string prefix = random_string(90); // 90 ký tự đầu giống nhau
                vector<string> suffixes;
                
                int current_chunk = min(chunk_size, n - i);
                for (int j = 0; j < current_chunk; ++j) {
                    suffixes.push_back(random_string(10));
                }
                // Sắp xếp ngược 10 ký tự cuối
                sort(suffixes.rbegin(), suffixes.rend()); 
                
                for (const string& suf : suffixes) {
                    data.push_back(prefix + suf);
                }
            }
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 3: {
            // Test 3: Đảo ngược hoàn toàn (Độ dài max 100)
            vector<string> data;
            data.reserve(n);
            for (int i = 0; i < n; ++i) {
                data.push_back(random_string(100));
            }
            sort(data.rbegin(), data.rend());
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 4: {
            // Test 4: Long Common Prefix (99 ký tự 'x', 1 ký tự cuối ngẫu nhiên)
            vector<string> data;
            data.reserve(n);
            string prefix(99, 'x');
            for (int i = 0; i < n; ++i) {
                data.push_back(prefix + (char)('a' + char_dist(gen)));
            }
            shuffle(data.begin(), data.end(), gen);
            for (const string& s : data) cout << s << "\n";
            break;
        }
        case 5: {
            // Test 5: Root Explosion (Ký tự đầu ngẫu nhiên, 99 ký tự sau là 'y')
            vector<string> data;
            data.reserve(n);
            string suffix(99, 'y');
            for (int i = 0; i < n; ++i) {
                data.push_back((char)('a' + char_dist(gen)) + suffix);
            }
            shuffle(data.begin(), data.end(), gen);
            for (const string& s : data) cout << s << "\n";
            break;
        }
        default:
            cerr << "Error: Invalid test_id for 'strlenlexi' (Must be 1-5)\n";
    }
}

// ==========================================
// HÀM MAIN
// ==========================================
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Kiểm tra số lượng tham số
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <int|strlexi|strlenlexi> <1-5>\n";
        return 1;
    }

    string type = argv[1];
    int test_id = 0;

    // 2. Parse ID an toàn 
    try {
        test_id = stoi(argv[2]); 
    } catch (const invalid_argument& e) {
        cerr << "Error: Test ID must be a valid integer.\n";
        return 1;
    } catch (const out_of_range& e) {
        cerr << "Error: Test ID is out of bounds.\n";
        return 1;
    }

    // 3. Kiểm tra logic ID
    if (test_id < 1 || test_id > 5) {
        cerr << "Error: Test ID must be between 1 and 5.\n";
        return 1;
    }

    // 4. Routing
    if (type == "int") {
        generate_int_test(test_id);
    } 
    else if (type == "strlexi") {
        generate_strlexi_test(test_id);
    } 
    else if (type == "strlenlexi") {
        generate_strlenlexi_test(test_id);
    } 
    else {
        cerr << "Error: Unknown benchmark type '" << type << "'.\n";
        return 1;
    }

    return 0;
}