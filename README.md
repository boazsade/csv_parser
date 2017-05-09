# csv_parser
a parser for CSV formatted input (any type of stream)

The code in this project require a compiler with support for C++14

This code would parse CSV formatted input in the form of C++ like stream - the stream can be costomzied to be C++ like stream
but it is not a requirement in the code itself, only a templated default.
a code example can be found under the example directory but in general - using iterators interface it would look like this:

struct matcher
{
    matcher(int i, const std::string& w) : index(i), what(w)
    {
    }

    bool operator () (const csv::cells_iterator<csv::parser>& begin) const
    {
        auto at = std::next(begin, index);
        return *at == what;
    }

    int index = 0;
    const std::string what;
};


void count_csv(std::ifstream& input, const std::string& what, int index)
{
    csv::parser::source_type buffer{input};
    csv::parser parser;
    matcher m(index, what);
    auto count = std::count_if(csv::make_rows_iterator(parser, buffer),
                        csv::make_rows_iterator_end(parser, buffer), m);
    std::cout<<"we found "<<count<<" entries for "<<what<<std::endl;
}
note that this would use default tokens delimiter of ',' char and '"' as escape for quation of string
