#include "libs/csv_parser/csv_iterator.h"
#include "libs/csv_parser/csv_parser.h"
#include "libs/csv_parser/csv_parser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iterator>


void process(std::istream& in, const char* name)
{
        //csv::parser::source_type buffer{uds::read_stream_into_container(in)};
        csv::parser::source_type buffer{in};
        csv::parser parser;
        csv::rows_iterator<csv::parser> be{buffer, parser};
        csv::rows_iterator<csv::parser> e;
        std::size_t cells = 0;
        int rows = 0;
        std::vector<int> info;
        info.resize(200);
        std::fill_n(std::begin(info), 200, 0);
        for(; be != e; ++be) {
            auto re = csv::cells_iterator<csv::parser>{};
            ++rows;
            for (auto row = *be; row != re; ++row) {
                ++cells;
            }
        }
        std::cout<<"processed "<<rows<<" rows from "<<name<<" with fields: "<<cells<<std::endl;
        int i = 0;
        std::for_each(std::begin(info), std::end(info), [&i] (auto entry) { 
                if (entry != 0) { std::cout<<"["<<i<<"] = "<<entry<<std::endl; 
            } ++i; 
         }
        );
    

}

void fix_csv(std::istream& is, int col_count)
{
   // csv::parser::source_type buffer{uds::read_stream_into_container(is)};
    csv::parser::source_type buffer{is};
    csv::parser parser;
    std::vector<csv::parser::token_type> one_row;
    std::for_each(csv::make_rows_iterator(parser, buffer), 
                    csv::make_rows_iterator_end(parser, buffer), 
                        [col_count, &one_row] (auto row_it) {
                            csv::cells_iterator<csv::parser> re;
                            std::copy(row_it, re, std::back_inserter(one_row));
                            if ((int)one_row.size() == col_count) {
                                std::copy(std::begin(one_row), std::end(one_row)-1, std::ostream_iterator<csv::parser::token_type>(std::cout, ","));
                                std::cout<<one_row[col_count-1]<<"\n";
                            }
                            one_row.clear();
                        }
                    );


}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr<<argv[0]<<"usage <input csv file> [expected number of columns]<\n";
        return -1;
    }

    std::ifstream in{argv[1]};
    if (!in) {
        std::cerr<<"failed to read "<<argv[1]<<std::endl;
        return -1;
    }

    if (argc == 2) {
        process(in, argv[1]);
    } else {
        auto ec = atoi(argv[2]);
        if (ec != 0) {
            fix_csv(in, ec);        
        } else {
            std::cerr<<"invalid number of columns "<<argv[2]<<std::endl;
        }
    }

}

