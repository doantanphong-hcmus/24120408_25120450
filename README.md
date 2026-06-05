# BÁO CÁO ĐỒ ÁN SORT BENCHMARK

## I. THÔNG TIN CHUNG
* **Môn học:** Cấu trúc dữ liệu và Giải thuật (DSA)
* **Thời gian học:** Học kỳ II, Năm học 2025 - 2026
* **Nhóm sinh viên thực hiện:**
 Đoàn Tấn Phong (24120408)  -  Võ Ngọc Bảo Trân (25120450)
* **Lớp:** 25CTT6
* **Trường:** Đại học Khoa học Tự nhiên - ĐHQG-HCM (HCMUS)

---

## II. LẦN CHẠY THỨ NHẤT: TỐI ƯU HÓA HIỆU NĂNG BAN ĐẦU 

### 1. Thuật toán cài đặt tốt nhất trong Lần chạy 1
Trong đợt nộp bài đầu tiên, chiến lược của nhóm là phát triển song song hai hướng thuật toán: **Radix Sort** (Thuật toán sắp xếp theo cơ số tuyến tính) và **Sedgewick's Optimized Quicksort** (Quicksort tối ưu hóa cấu trúc phân hoạch). Qua các đợt kiểm thử nghiêm ngặt nội bộ trên cấu hình bộ test tiêu chuẩn do giáo viên cung cấp, nhóm nhận thấy **Radix Sort** mang lại hiệu năng áp đảo tuyệt đối ở cả 3 bài toán. Do đó, nhóm quyết định chốt Radix Sort làm giải pháp nộp chính thức cho Lần chạy 1.

Các phương thức tối ưu hóa cốt lõi được áp dụng bao gồm:
* **Bài A (Integer Sort):** Áp dụng Radix Sort cơ số 256 với đúng 4 lượt quét (passes) trên kiến trúc số nguyên 32-bit. Để xử lý mảng chứa cả số nguyên âm và số dương mà không làm đảo lộn thứ tự tuyến tính, nhóm sử dụng kỹ thuật thao Bitwise Manipulation: `x ^ 0x80000000`. Phép toán này đảo ngược bit dấu, chuyển đổi toàn bộ dải số nguyên có dấu sang dải số nguyên không dấu tương đương một cách tuần tự, giúp Radix Sort phân bổ bucket chính xác tuyệt đối mà không phát sinh chi phí rẽ nhánh `if-else`.
* **Bài B & C (String Sort):** Radix Sort được tinh chỉnh cấu trúc bộ nhớ đệm. Tuy nhiên, để đạt tốc độ cực hạn, nhóm đã xây dựng hệ thống **Custom Fast I/O**. Toàn bộ dữ liệu đầu vào được nạp trực tiếp vào một vùng đệm tĩnh khổng lồ (`in_buf`), triệt tiêu hoàn toàn overhead gọi hàm của `std::cin` hay `scanf`. Dữ liệu xuất ra cũng được gom vào `out_buf` trước khi đẩy ra luồng xuất chuẩn bằng `cout.write`.

### 2. Lý do Radix Sort tối ưu nhất ở Lần chạy 1
* **Độ phức tạp tuyến tính tuyệt đối:** Trong điều kiện bộ test phân bổ ngẫu nhiên hoặc theo phân phối chuẩn của thầy, Radix Sort đạt độ phức tạp thời gian $O(N \cdot K)$ (với $K$ là số pass cố định hoặc chiều dài chuỗi). Khác với Quicksort phụ thuộc nặng nề vào cách chọn phần tử chốt để tránh suy biến thành $O(N^2)$, Radix Sort giữ vững tốc độ xử lý ổn định trong mọi tình huống phân bổ dữ liệu thông thường.
* **Hằng số tối thiểu nhờ loại bỏ hàm thư viện:** Bằng cách tự quản lý mảng tần suất `cnt` và mảng phụ `b` trên vùng nhớ tĩnh, thuật toán loại bỏ hoàn toàn các thao tác cấp phát động của `std::vector` hay `std::string`, giảm thiểu tối đa hiện tượng trễ do phân mảnh bộ nhớ.

---

## III. CHIẾN LƯỢC SINH DỮ LIỆU THỬ NGHIỆM

### 1. Phương pháp và Triết lý sinh test case
Dựa trên phân tích chiến thuật tổng thể, nhóm dự đoán phần lớn các nhóm khác trong lớp sẽ lựa chọn **Radix Sort** cơ bản hoặc **Quicksort thuần túy** (chỉ chọn pivot đầu/cuối) làm thuật toán chủ đạo cho Lần chạy 1 vì tính dễ cài đặt và tốc độ cao của chúng trên test chuẩn. 

Vì vậy, triết lý xây dựng file `test_gen.cpp` của nhóm là **"Sinh bộ test đặc hiệu chống lại Radix Sort và Quicksort sơ sài"**. Nhóm chấp nhận một rủi ro chiến thuật cực kỳ lớn: **Bộ test này cũng sẽ khiến chính mã nguồn Radix Sort của nhóm ở Lần 1 chạy chậm đi rất nhiều**. Tuy nhiên, đây là một bước lùi chủ đích nhằm triệt hạ điểm số tốc độ của tất cả nhóm khác sử dụng Radix Sort, tạo tiền đề để nhóm có thể vươn lên ở Lần chạy 2 bằng một thuật toán dự phòng kín kẽ hơn.

### 2. Chi tiết các thuật toán mục tiêu và Cơ chế phá hoại của bộ test
Bộ test được thiết kế gồm 5 cases cho mỗi bài toán, nhắm thẳng vào các điểm nghẽn hệ thống:

#### MODULE 1: INTERGER SORT (int) - Mục tiêu: Quicksort thuần và Radix Sort kém quản lý cache
* **Test 1 (Cực hạn số âm/dương/0) & Test 3 (Alternating Min/Max):** Lặp lại chuỗi tuần hoàn `[MAX, MIN, 1, -1]` liên tục. Bộ dữ liệu này ép các thuật toán Radix Sort sử dụng cơ số không tối ưu hoặc không xử lý kỹ bit dấu sẽ bị tràn mảng hoặc tính toán sai chỉ mục bucket. Đối với Quicksort, sự chênh lệch biên độ cực lớn làm mất tính đối xứng khi phân hoạch.
* **Test 2 (Mảng toàn số 0) & Test 4 (Mảng nghịch thế có trùng lặp lớn):** Đây là nút thắt cổ chai dành cho các nhóm cài đặt Quicksort sử dụng phân hoạch Lomuto hoặc Hoare truyền thống mà không có cơ chế phân hoạch 3 hướng (3-way partition). Khi gặp mảng toàn phần tử trùng lặp, các phần tử chốt sẽ chia mảng thành hai phần cực kỳ mất cân bằng (một bên trống và một bên chứa $N-1$ phần tử), ép độ phức tạp thời gian rơi thẳng từ $O(N \log N)$ xuống $O(N^2)$, gây ra lỗi vượt quá giới hạn thời gian (TLE).
* **Test 5 (Mảng ngẫu nhiên hoàn toàn - Stress Test):** Khởi tạo $10^5$ phần tử với giá trị ngẫu nhiên phân bổ đều trong toàn bộ không gian số nguyên 32-bit (từ $-2^{31}$ đến $2^{31}-1$). Test case này không gài bẫy trường hợp xấu nhất, mà là bài kiểm tra thuần túy về hiệu năng cơ sở (Average-case). Nó đánh giá mức độ thân thiện với bộ nhớ đệm (Cache-friendly) của Radix Sort và chi phí phân nhánh (Branch prediction) của Quicksort trên một tập dữ liệu hỗn loạn hoàn toàn.

#### MODULE 2: LEXICOGRAPHIC SORT (strlexi) - Mục tiêu: MSD Radix Sort và Insertion Sort Fallback
* **Test 1 (Tất cả chuỗi giống hệt nhau):** Khởi tạo toàn bộ $10^5$ chuỗi hoàn toàn giống nhau (toàn chữ 'a', độ dài 100). Test case này nhắm vào các thuật toán Quicksort cài đặt sơ sài không có phân hoạch 3 hướng. Khi tất cả phần tử bằng nhau, quá trình phân hoạch sẽ chia mảng cực kỳ mất cân bằng, ép độ phức tạp suy biến về $O(N^2)$.
* **Test 2 (Mảng đã được sắp xếp sẵn):** Khởi tạo $10^5$ chuỗi ngẫu nhiên (độ dài 100) và đã được sắp xếp tăng dần. Đây là trường hợp "Best-case" theo lý thuyết của một số thuật toán, nhưng lại là cạm bẫy (Worst-case) khiến các bản cài đặt Quicksort sơ sài (chọn phần tử chốt cố định ở đầu hoặc cuối) bị tràn ngăn xếp đệ quy và chạy chậm kỷ lục.
* **Test 3 (Mảng sắp xếp ngược hoàn toàn):** Sinh mảng $10^5$ chuỗi ngẫu nhiên và được sắp xếp giảm dần. Tương tự Test 2, dữ liệu nghịch thế hoàn toàn sẽ triệt tiêu cơ chế chọn chốt ngây thơ của Quicksort, đồng thời tối đa hóa số lần hoán đổi dữ liệu, kiểm chứng tốc độ gán con trỏ bộ nhớ của thuật toán.
* **Test 4 (Tấn công mạnh vào insertion sort):** Khi sắp xếp chuỗi ký tự, các thuật toán chia để trị nâng cao như MSD (Most Significant Digit) Radix Sort hoặc String Quicksort thường cấu hình một ngưỡng gọi là *Fallback Threshold* (thường là $N < 32$). Khi mảng con nhỏ hơn ngưỡng này, hệ thống sẽ chuyển sang dùng **Insertion Sort** để giảm chi phí đệ quy. Nhóm đã sinh dữ liệu bằng cách nhóm các block 31 chuỗi có **90 ký tự đầu giống hệt nhau**, nhưng **10 ký tự cuối bị đảo ngược hoàn toàn**. Khi MSD Radix Sort phân rã mảng đến ngưỡng 31, nó sẽ chuyển sang Insertion Sort. Tại đây, Insertion Sort gặp mảng đảo ngược sẽ phải thực hiện số phép hoán đổi tối đa, đẩy chi phí cục bộ lên $O(31^2)$ cho mỗi mảng con. Với $N=10^5$, tổng thời gian chạy bị kéo dài đáng kể.
* **Test 5 (Tiền tố chung cực dài - LCP):** Sinh ra $10^5$ chuỗi có 99 ký tự đầu tiên là chữ `a`, ký tự thứ 100 mới ngẫu nhiên. Bộ test này phá vỡ hoàn toàn tính hiệu quả của Radix Sort thông thường, ép thuật toán phải đệ quy sâu xuống tận tầng thứ 100, tiêu tốn tài nguyên bộ nhớ đệm và làm suy giảm tốc độ đọc ghi dữ liệu do phải so sánh đến byte cuối cùng.

#### MODULE 3: LENGTH-AWARE SORT (strlenlexi) - Mục tiêu: Thuật toán phân nhánh sớm (Trie/MSD)
* **Test 1 (Mảng trùng lặp cực đại):** $10^4$ chuỗi hoàn toàn giống nhau (toàn chữ 'a', độ dài 100). Bộ test này đo lường sự lãng phí chi phí cấp phát vùng nhớ hay bucket dư thừa của Counting/Radix Sort, đồng thời trừng phạt các thuật toán Quicksort không xử lý tối ưu tập dữ liệu trùng lặp.
* **Test 2 (Insertion Sort Killer - Ngưỡng N < 25):** Vẫn sử dụng nguyên lý tấn công vào Fallback Threshold, nhưng lần này cấu hình kích thước cụm (chunk size) thành 24. Cụm 24 chuỗi được thiết kế với 90 ký tự đầu giống nhau và 10 ký tự cuối sắp xếp ngược để vô hiệu hóa thuật toán Insertion Sort được gọi ở cuối đệ quy.
* **Test 3 (Đảo ngược hoàn toàn):** Sinh ra $10^4$ chuỗi ngẫu nhiên (độ dài 100) và sắp xếp giảm dần. Đây là trường hợp xấu để kiểm chứng chi phí hoán vị bộ nhớ của các thuật toán phân hoạch.
* **Test 4 (Long Common Prefix - LCP):** Toàn bộ $10^4$ chuỗi có 99 ký tự đầu tiên là chữ `x`, chỉ khác biệt ngẫu nhiên ở ký tự cuối cùng. Mảng sau đó được xáo trộn ngẫu nhiên. Test này vô hiệu hóa lợi thế nhóm theo chiều dài, ép mọi chuỗi có cùng cấu trúc phải được so sánh đệ quy rất sâu, làm cạn kiệt tài nguyên xử lý.
* **Test 5 (Root Explosion):** Thiết kế chuỗi có ký tự đầu tiên ngẫu nhiên (`a-z`), nhưng toàn bộ 99 ký tự phía sau đều là chữ `y`. Đối với các thuật toán dựa trên cấu trúc cây phân nhánh từ gốc như Trie hoặc MSD Radix Sort, bộ test này ép cấu trúc cây phải phình to ngay từ tầng đầu tiên (tạo ra 26 nhánh), nhưng các tầng phía sau lại co cụm lại thành một đường thẳng đơn độc. Việc này làm tăng tối đa overhead khởi tạo và quản lý con trỏ node nhưng lại không mang lại hiệu quả thu hẹp phạm vi tìm kiếm dữ liệu.

---

## IV. LẦN CHẠY THỨ HAI: CẢI TIẾN THUẬT TOÁN VÀ KHẮC PHỤC LỖ HỔNG 

### 1. Thuật toán cài đặt tốt nhất trong Lần chạy 2
Đúng như nhóm đã tính toán ở giai đoạn sinh test, bước sang Lần chạy 2, các bộ test cực đoan đã bộc lộ giới hạn của các bản cài đặt Lần 1. Để lật ngược tình thế, nhóm quyết định chuyển đổi kiến trúc sắp xếp chuỗi sang **Sedgewick's Optimized Quicksort** nhằm chống lại các test case có tiền tố chung cực dài và cấu trúc nghịch thế.

Chiến thuật phân bổ cụ thể cho từng bài ở Lần 2 như sau:
* **Bài A (Integer Sort):** Nhóm nhận thấy thuật toán Radix Sort 4 passes kết hợp phép đảo bit dấu ở Lần 1 vẫn hoạt động cực kỳ ổn định và chống chịu tốt trước mọi dạng biến động của mảng số nguyên. Tốc độ thực tế so với các nhóm dẫn đầu không chênh lệch đáng kể. Do đó, nhóm quyết định **giữ nguyên mã nguồn Bài A**, không thay đổi thuật toán để bảo toàn sự ổn định, tập trung toàn lực tài nguyên cho hai bài toán chuỗi.
* **Bài B (Lexicographic Sort):** Chuyển từ Radix Sort sang thuật toán **3-way String Quicksort** tối ưu theo lý thuyết của Robert Sedgewick. Thuật toán phân chia mảng thành 3 phần rõ rệt (nhỏ hơn, bằng, và lớn hơn pivot) dựa trên ký tự tại độ sâu `depth`. Để chọn chốt (pivot) an toàn và nhanh chóng, nhóm sử dụng kỹ thuật **Median-of-3** (chọn phần tử trung vị của 3 phần tử đầu, giữa và cuối mảng).
* **Bài C (Length-aware Lexicographic Sort):** Triển khai mô hình **Counting Sort theo chiều dài trước** (đếm tần suất bằng mảng `cnt` và chia `bucket`). Sau đó, tiến hành gọi **3-way String Quicksort** cục bộ cho từng nhóm chuỗi có cùng độ dài. Khác với Bài B, thuật toán Quicksort ở Bài C được tích hợp thêm **Randomized Pivot** (`fast_rand`) trước khi lấy Median-of-3, giúp bẻ gãy hoàn toàn các bộ test cố tình gài bẫy trường hợp xấu nhất.

### 2. Các phương thức tối ưu nâng cao so với Lần chạy 1
Để đảm bảo Sedgewick's Quicksort đạt tốc độ vượt trội và bẻ gãy các bộ test phá hoại, nhóm áp dụng các kỹ thuật tối ưu hóa chuyên sâu:

* **Quản lý con trỏ mảng tĩnh và Kỹ thuật Zero-copy:** Thay vì dùng kiểu dữ liệu `std::string` (liên tục kích hoạt cấp phát động và sao chép sâu), nhóm dùng mảng con trỏ tĩnh `const char* arr[]`. Vùng đệm đầu vào `in_buf` được duyệt qua, nhóm tự động đè các khoảng trắng và ký tự xuống dòng bằng ký tự kết thúc chuỗi `\0` (null-terminator). Mảng `arr` chỉ lưu địa chỉ của chuỗi. Quá trình sắp xếp thực chất chỉ là hoán đổi vị trí các con trỏ 8-byte (`swap_ptr`), đạt hiệu suất tiệm cận phần cứng.
* **Cơ chế Fallback Insertion Sort:** Nhóm thiết lập ngưỡng cắt tỉa (cutoff) cho Quicksort. Khi mảng con đệ quy phân tách xuống kích thước đủ nhỏ ($N \le 16$), thuật toán dừng đệ quy và chuyển sang gọi `final_insertion_sort`. Vì Insertion Sort có hằng số rất nhỏ trên mảng bé, kỹ thuật này giúp giảm triệt để overhead (chi phí chìm) do gọi hàm đệ quy liên tục.
* **Randomized Pivot (fast_rand) và Median-of-3:** Kỹ thuật chọn phần tử chốt được nâng cấp toàn diện. Bài B dùng **Median-of-3** để tăng tính đối xứng. Tại Bài C, nhóm tự viết hàm sinh số ngẫu nhiên siêu tốc `fast_rand()` dựa trên thuật toán thao tác bit Xorshift. Hàm này chỉ mất vài chu kỳ xung nhịp để sinh ra chốt ngẫu nhiên, hoàn toàn vượt trội so với `rand()` chuẩn của C++, triệt tiêu mọi nguy cơ suy biến về độ phức tạp $O(N^2)$ của Quicksort.

---

## V. KẾT LUẬN
Đồ án thi đấu DSA đã mang lại một góc nhìn thực chiến sâu sắc về tối ưu hóa mã nguồn. Kết quả hai lần chạy chứng minh rằng: không có một thuật toán nào là vạn năng trong mọi hoàn cảnh. **Radix Sort** là vô đối về tốc độ trên dữ liệu đồng đều, nhưng dễ dàng bị đánh bại bởi các bộ test tiền tố dài. Trong khi đó, **Sedgewick's Optimized Quicksort** với cơ chế chọn pivot thông minh, phân hoạch 3 hướng và cấu trúc Zero-copy đã chứng minh được sự lì đòn và khả năng bảo vệ hệ thống tuyệt đối trước các đợt tấn công test case ở Lần chạy 2, giúp nhóm tối ưu hóa điểm số một cách bền vững.

---
