/*
*  C++ Implementation: dot2Vhdl
*
* Description:
*
*
* Author: Andrea Guerrieri <andrea.guerrieri@epfl.ch (C) 2019
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "stdlib.h"
#include <string.h>
#include "dot2hdl.h"
#include "dot_parser.h"
#include "vhdl_writer.h"
#include "eda_if.h"
#include "lsq_generator.h"
#include <regex>

using namespace std;



#include "table_printer.h"

using bprinter::TablePrinter;

// void report_instances ( void )
// {
// 
//     cout << endl << "Report Modules "<< endl;
// 
//     TablePrinter tp(&std::cout);
//     tp.AddColumn("Node_ID", 10);
//     tp.AddColumn("Name", 20);
//     tp.AddColumn("Module_type", 20);
//     tp.AddColumn("Inputs", 10);
//     tp.AddColumn("Outputs", 10);
// 
//     tp.PrintHeader();
// 
//     for (int i = 0; i < components_in_netlist; i++) 
//     {
//         tp << i << nodes[i].name << nodes[i].type << nodes[i].inputs.size << nodes[i].outputs.size;
//     }
//     
//     tp.PrintFooter();
// 
// }

void report_instances ( void )
{

    cout << endl << "Report Modules "<< endl;

    TablePrinter tp(&std::cout);
    tp.AddColumn("Node_ID", 10);
    tp.AddColumn("Name", 20);
    tp.AddColumn("Module_type", 20);
    tp.AddColumn("Inputs", 10);
    tp.AddColumn("Outputs", 10);

    tp.PrintHeader();

    for (int i = 0; i < components_in_netlist; i++) 
    {
        tp << i << nodes[i].name << nodes[i].type << nodes[i].inputs.size << nodes[i].outputs.size;
    }
    
    tp.PrintFooter();

}

void report_instances_external (string filename ) //Carmine 08.02.2022 adding report list cells
{
    ofstream file;
    file.open(filename);

    for (int i = 0; i < components_in_netlist; i++) 
    {
        file << nodes[i].name << endl;
    }
    file.close();

}


//Carmine 22.02.22 This vector defines a table of mixed connections between pins of different timing domains inside a block
//
//                   If you want to modify it you should change his copy also in Buffers folder in the DFnetlist_buffers.cpp file
//                      In this file there is also a detailed explanation of this table
//
//                      This version of the table is mainly used to easily
//
vector<pair <string, string> > MixedConnectionBlock ={
    {"Operator", "vr=v0r1/v1r0"},
    {"Branch", "cv=d1v*_cr=d1v*_vr=v0r1/v1r0"},
    {"CntrlMerge", "vc=v0d1"},
    {"Exit", "vr=v0r1/v1r0"},
    {"Merge", "vd"},
    {"Mux", "cr=d0r*_cv=d0v*_vd=v*d*_vr=v*r*"}
};

void report_instances_external_conn_mixed (string filename ) //Carmine 2.02.2022 adding report list cells with mixed connections
{
    ofstream file;
    file.open(filename);

    for (int i = 0; i < components_in_netlist; i++) 
    {
        for(pair<string, string> x: MixedConnectionBlock){
            if(x.first.compare(nodes[i].type) == 0){
                file << nodes[i].name;
                string line = x.second, word, conn;
                regex rg_1("_");
                regex rg_2("=");
                sregex_token_iterator rg_iter_1(line.begin(), line.end(), rg_1, -1);
                sregex_token_iterator end_iter;
                for(; rg_iter_1 != end_iter; rg_iter_1++){
                    word = *rg_iter_1;
                    sregex_token_iterator rg_iter_2(word.begin(), word.end(), rg_2, -1);
                    conn = *rg_iter_2;
                    file << " " << conn;
                }
                file << endl;
                continue;
            }
        }
    }
    file.close();

}


void report_area ( void )
{

    cout << "Report Estimated Area \n\r";

    int total_area = 0;
    
    TablePrinter tp(&std::cout);
    tp.AddColumn("Node_ID", 8);
    tp.AddColumn("Name", 18);
    tp.AddColumn("Module_type", 20);
    tp.AddColumn("Area", 10);

    tp.PrintHeader();

    for (int i = 0; i < components_in_netlist; i++) 
    {
        tp << i << nodes[i].name << nodes[i].type << 123;
        total_area+=123;
    }
    tp.PrintFooter();
    tp << "" << "" << "Total"  << total_area;
    tp.PrintFooter();
}


void print_netlist ( void )
{
    static int indx;
    cout << "Printing out Netlist" << endl;
    
    for (int i = 0; i < components_in_netlist; i++) 
    {

        cout << "[Node ID: " << i << "]";
        cout << "[Name: " << nodes[i].name<< "]"; 
        cout << "[Type: " << nodes[i].type<< "]"; 
        cout << "[Operator: " << nodes[i].component_operator<< "]"; 
        cout << "[Value: " << nodes[i].component_value<< "]"; 
        cout << "[Inputs: " << nodes[i].inputs.size; 
        for (  indx = 0; indx < nodes[i].inputs.size; ++indx)
        {
            cout << " Bitsize: " << nodes[i].inputs.input[indx].bit_size; 
            cout << " Input: " << indx << " Connected to node :"  << nodes[nodes[i].inputs.input[indx].prev_nodes_id].name; 
        }
        cout << "]";
        cout << "[Outputs: " << nodes[i].outputs.size ; 
        for ( indx = 0; indx < nodes[i].outputs.size; ++indx)
        {
            cout << " Bitsize: " << nodes[i].outputs.output[indx].bit_size;
            cout << " Output: " << indx << " Connected to :"  << nodes[nodes[i].outputs.output[indx].next_nodes_id].name; 

        }
        cout << "]"; 
        cout << endl << endl;
    }
    
    
    report_instances ( );
//    report_area ( );
}   

