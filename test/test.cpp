#include <iostream>
#include <forward_list>
#include <random>
#include <clany/utilities.h>
#include <clany/algorithm.hpp>
#include <clany/dyn_bitset.hpp>

using namespace std;
using namespace cls;

_CLANY_BEGIN
struct Shape {
    using Ptr = shared_ptr < Shape > ;

    virtual void draw() { cout << "drawing a shape" << endl; }
};

struct Rect : Shape {
    Rect(int width, int height)
        : w(width), h(height) {};
    Rect(int width)
        : w(width), h(width) {};

    void draw() override {
        cout << "drawing a Rect, width: " << w << " height: " << h << endl;
    }

    int w, h;
};

struct Square final : Rect {
    Square(int width)
        : Rect(width), w(width) {};

    void draw() override {
        cout << "drawing a square, width: " << w << endl;
    }

    int w;
};
_CLANY_END

struct CmdArgs {
    string name       = "";
    string address    = "";
    int    post_code  = 0;
    bool   need_print = false;
    string load_file  = "";
};

void algTest()
{
    ScopeTimer scp_timer;

    vector<int> vec1 {9, 3, 5, 7, 9, 13, 9, 17, 17, 9};
    int arr1[] = {9, 3, 5, 7, 3, 11, 9, 1, 13, 9};
    bool all_odd = all_of(vec1, [](int ele) { return ele % 2 == 1; });
    ASSERT(all_odd);
    all_odd = none_of(arr1, [](int ele) { return ele % 2 == 0; });
    ASSERT(all_odd);

    for_each({7, 5, 9, 15, 3}, [](int ele) { cout << ele << " "; });
    cout << endl;

    int nine_num = count(vec1, 9);
    ASSERT(4 == nine_num);
    int divide_by_three = count_if(arr1, [](int ele) { return ele % 3 == 0; });
    ASSERT(5 == divide_by_three);

    auto non_eq = mismatch(vec1, arr1, less_equal<int>());
    DBGVAR(cout, *non_eq.first);
    DBGVAR(cout, *non_eq.second);

    auto vec2 = vec1;
    ASSERT(equal(vec1, vec2));
    ASSERT(equal(vec1, arr1, greater_equal<int>()));

    auto iter = find(arr1, 11);
    ASSERT(iter != end(arr1));
    iter = find_if(arr1, [](int ele) { return ele > 9; });
    DBGVAR(cout, *iter);
    auto vec_iter = find_first_of(vec1, {11, 15, 17}, greater<int>());
    ASSERT(5 == distance(begin(vec1), vec_iter));

    vec_iter = adjacent_find(vec1);
    ASSERT(7 == distance(begin(vec1), vec_iter));
    iter = adjacent_find(arr1, [](int ele1, int ele2) { return (ele2 - ele1) == 8; });
    ASSERT(4 == distance(begin(arr1), iter));

    iter = search(arr1, {11, 9, 1});
    ASSERT(5 == distance(begin(arr1), iter));
    vec_iter = search_n(vec1, 6, 7, greater<int>());
    ASSERT(4 == distance(begin(vec1), vec_iter));

    copy({1, 2, 3}, vec2.begin());
    copy(arr1, vec2);
    ASSERT(equal(arr1, vec2));
    copy_if(vec1, vec2, [](int ele) { return ele >= 13; });
    ASSERT(equal({13, 17, 17}, vec2));
    vec2 = {5, 7, 3, 11, 9, 1, 13};
    copy_backward({13, 15, 15}, vec2.end());
    ASSERT(3 == distance(search(vec2, {13, 15, 15}), end(vec2)));

    move_backward(vec2, end(arr1));

    fill(arr1, -1);
    ASSERT(all_of(arr1, [](int ele) { return ele == -1; }));

    transform(vec2, begin(arr1), negate<int>());
    transform({1, 3, 5}, vec1, negate<int>());
    ASSERT(equal(vec2, arr1, [](int ele1, int ele2) {
        return abs(ele1) == abs(ele2);
    }));

    const uint SEED = random_device()();
    auto  rd_engine = default_random_engine(SEED);
    generate(arr1, rd_engine);
    remove_copy_if(arr1, vec1, [](int ele) { return ele % 2 != 0; });
    rotate_copy(vec2, 3, begin(arr1));

    DBGVAR(cout, accumulate(vec1) + 0.1f);
    DBGVAR(cout, accumulate(arr1, 1, [](int ele, int init) {
        return ele*init;
    }));

    vector<float> vec3(vec1.size());
    transform(arr1, vec3, [](int ele) { return ele + 0.3; });
    DBGVAR(cout, inner_product(vec1, vec3));

    auto inprod = inner_product(vec1, vec3,
                                [](float ele1, float ele2) { return ele1 + ele2; },
                                [](int   ele1, float ele2) { return ele1 / ele2; });
    DBGVAR(cout, inprod);

    swap(vec1[3], vec1[5]);
    partial_sort(vec1, 3);
    ASSERT(4 == distance(vec1.begin(), is_sorted_until(vec1)));
    sort(vec1, greater<int>());

    auto minmax_val = minmax_element(arr1);
    DBGVAR(cout, *minmax_val.first);
    DBGVAR(cout, *minmax_val.second);
}

int main(/*int argc, char* argv[]*/)
{
    CPUTimer timer;

    algTest();

    timer.delta();

    BitField bit_field(13, "0110111010111");
    cout << bit_field << endl;
    for (auto i = 0; i < 13; ++i) {
        cout << bit_field[i];
    }
    cout << endl;

    // Command line parser
    // Test arguments: --name=Clany -a "441 S Dunn St Apt 5" -c47401 -p --file "..\CMakeLists.txt"
    int t_argc = 9;
    const char* t_argv[] = {
        "Path to executable",
        "--name=Clany",
        "-a",
        "441 S Dunn St Apt 5",
        "-c47401",
        "-p",
        "shit!",
        "--file",
        "..\\CMakeLists.txt"
    };

    vector<LongOption> long_options = {
        {"name"     , required_argument, 'n'},
        {"address"  , optional_argument, 'a'},
        {"post_code", required_argument, 'c'},
        {"print"    , no_argument      , 'p'},
        {"file"     , required_argument, 'f'},
    };
    CmdLineParser cmd_parser(t_argc, t_argv, "n:a::c:pf:", long_options);

    CmdArgs cmd_args;
    char ch;
    while ((ch = cmd_parser.get()) != -1) {
        switch (ch) {
        case 'n':
            cmd_args.name = cmd_parser.getArg<string>();
            break;
        case 'a':
            cmd_args.address = cmd_parser.getArg<string>();
            break;
        case 'c':
            cmd_args.post_code = cmd_parser.getArg<int>();
            break;
        case 'p':
            cmd_args.need_print = true;
            break;
        case 'f':
            cmd_args.load_file = cmd_parser.getArg<string>();
            break;
        case ':':
            cerr << "ERROR: Invalid option, missing argument!" << endl;
            exit(1);
        case '?':
            cerr << "ERROR: Unknown argument!" << endl;
            exit(1);
        default:
            cerr << "ERROR: Parsing fail!" << endl;
            exit(1);
            break;
        }
    }

    if (cmd_args.need_print) {
        cout.setf(ios::left);
        cout << setw(9) << "name"      << ": " << cmd_args.name      << endl;
        cout << setw(9) << "address"   << ": " << cmd_args.address   << endl;
        cout << setw(9) << "post code" << ": " << cmd_args.post_code << endl;
    }

    // File operation
    if (!cmd_args.load_file.empty()) {
        TRACE("Loaded file name: %s", cmd_args.load_file.c_str());

        auto text = readFile(cmd_args.load_file);       // Read text file, save to string
        auto data = readBinaryFile(cmd_args.load_file); // Read binary file, save to ByteArray

        int line_num = countLine(cmd_args.load_file);
        text = getLineStr(cmd_args.load_file, line_num / 6);
        TRACE("%s", text.c_str());
    }

    // Factory method
    Shape::Ptr shape;
    Factory<Rect> rect_factory;
    shape = rect_factory(16, 10);
    shape = rect_factory(9);

    using ShapeFactory = ObjFactory<Shape, string, Shape::Ptr(int)>;
    ShapeFactory::addType("Rect", Factory<Rect>());
    ShapeFactory::addType("Square",    Factory<Square>());

    shape = ShapeFactory::create("Rect", 7);
    shape->draw();
    shape = ShapeFactory::create("Square", 9);
    shape->draw();

    ShapeFactory::removeType("Square");
    shape = ShapeFactory::create("Square", 5);
    if (!shape) cerr << "could not find type Square" << endl;

    timer.elapsed();

    // Iterator type traits
//     static_assert(is_iterator<Shape>::value, "Shape is not an iterator");
//     static_assert(is_iterator<Shape*>::value, "Shape* is not an iterator");
//     static_assert(is_const_iterator<const int>::value, "const int is not a const iterator");
//     static_assert(is_const_iterator<const Shape*>::value, "const Shape* is not a const iterator");
//     static_assert(is_const_iterator<vector<int>::iterator>::value, "vector<int>::iterator is not a const iterator");
//     static_assert(is_const_iterator<vector<int>::const_iterator>::value, "vector<int>::const_iterator is not a const iterator");
//     static_assert(is_input_iterator<vector<int>::iterator&>::value, "vector<int>::iterator is not an input iterator");
//     static_assert(is_input_iterator<ostream_iterator<int>>::value, "ostream_iterator<int> is not an input iterator");
//     static_assert(is_output_iterator<ostream_iterator<int>>::value, "ostream_iterator<int> is not an output iterator");
//     static_assert(is_output_iterator<istream_iterator<int>>::value, "istream_iterator<int> is not an output iterator");
//     static_assert(is_output_iterator<vector<int>::const_iterator>::value, "vector<int>::const_iterator is not an output iterator");
//     static_assert(is_forward_iterator<forward_list<int>::iterator>::value, "forward_list<int>::iterator is not an forward iterator");
//     static_assert(is_bidirectional_iterator<forward_list<int>::iterator>::value, "forward_list<int>::iterator is not an bidirectional iterator");
//     static_assert(is_bidirectional_iterator<list<int>::iterator>::value, "list<int>::iterator is not an bidirectional iterator");
//     static_assert(is_random_access_iterator<forward_list<int>::iterator>::value, "forward_list<int>::iterator is not an random access iterator");
//     static_assert(is_random_access_iterator<vector<int>::const_iterator>::value, "vector<int>::const_iterator is not an random access iterator");
//     static_assert(is_random_access_iterator<Shape*>::value, "Shape* is not an random access iterator");

    return 0;
}