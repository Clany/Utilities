#include <forward_list>
#include "clany/utilities.h"

using namespace std;
using namespace clany;

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
    string name = "";
    string address = "";
    int    post_code = 0;
    bool   need_print = false;
    string load_file = "";
};

int main(int argc, char* argv[])
{
    CPUTimer timer;

    // Command line parser
    // Test arguments: --name=Clany -a "441 S Dunn St Apt 5" -c47401 -p --file "..\CMakeLists.txt"
    int t_argc = 9;
    char* t_argv[] = {
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
    timer.pause();

    // Iterator type traits
//     static_assert(is_iterator<Shape>::value, "Shape is not an iterator");
//     static_assert(is_iterator<Shape*>::value, "Shape* is not an iterator");
//     static_assert(is_const_iterator<const int>::value, "const int is not a const iterator");
//     static_assert(is_const_iterator<const Shape*>::value, "const Shape* is not a const iterator");
//     static_assert(is_const_iterator<vector<int>::iterator>::value, "vector<int>::iterator is not a const iterator");
//     static_assert(is_const_iterator<vector<int>::const_iterator>::value, "vector<int>::iterator is not a const iterator");
//     static_assert(is_input_iterator<vector<int>::iterator>::value, "vector<int>::iterator is not an input iterator");
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