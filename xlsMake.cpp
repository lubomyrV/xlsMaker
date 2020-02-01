#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>

int main()
{
    typedef std::vector<std::string> vectorString;
    typedef std::string string;
    typedef std::map<string, vectorString> mapStringVector;
    typedef std::map<string, string> book;
    typedef std::vector<book> books;
    
    
    string finalDataString {""};
    
    finalDataString += "<?xml version=\"1.0\"?>";
    finalDataString += "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n"; 
    finalDataString += " xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n";
    finalDataString += " xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n";
    finalDataString += " xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n"; 
    finalDataString += " xmlns:html=\"http://www.w3.org/TR/REC-html40\">";
    
    finalDataString += "\n";


    finalDataString += "<Worksheet ss:Name=\"Sheet1\">";
    finalDataString += "<Table>";
    finalDataString += "\n";

    mapStringVector contextMap;//Id by values
    
    vectorString headers {"Book name", "Author", "Price", "Genre"};
    vectorString apiNames {"Book_name__c", "Author__c", " ", "Genre__c" };
    
    contextMap.insert(std::make_pair("0", headers));
    
    
    books listOfBooks;
    
    book book1;
    book1.insert(std::make_pair("Id", "77"));
    book1.insert(std::make_pair("Book_name__c", "The Lord of the Rings"));
    book1.insert(std::make_pair("Author__c", "J. R. R. Tolkien"));
    book1.insert(std::make_pair("Genre__c", "Fantasy"));
    listOfBooks.push_back(book1);
    
    
    book book2;
    book2.insert(std::make_pair("Id", "325"));
    book2.insert(std::make_pair("Book_name__c", "The Martian Chronicles"));
    book2.insert(std::make_pair("Author__c", "Ray Bradbury"));
    book2.insert(std::make_pair("Genre__c", "Science fiction"));
    listOfBooks.push_back(book2);
    
    
    for (int i = 0; i < listOfBooks.size(); ++i)
    {
        book theBook = listOfBooks[i];
        std::cout << " writing Id: " << theBook.find("Id")->second << '\n';
        //if key exists - skip iteration
        if (contextMap.count(theBook.find("Id")->second) > 0)
        {
            std::cout << "continue\n";
            continue;
        }
            
        
        vectorString values;
        
        for (int i = 0; i < apiNames.size(); ++i)
        {
            string apiKey = apiNames[i];
            
            std::map<string, string>::iterator it = theBook.find(apiKey);
            
            // Check if element exists in map or not
            if (it != theBook.end())
            {
                string key = it->first;
                string value = it->second;
                //std::cout << "key = " << key << " :: Value = " << value << std::endl;
                values.push_back(value);
            }
            else
            {
                //std::cout << "NOT found apiKey = " << apiKey << '\n';
                values.push_back("");
            }
            
        }
                
        contextMap.insert(std::make_pair(theBook.find("Id")->second, values));

    }
    

    for( std::map<string, vectorString>::iterator map_it = contextMap.begin(); map_it != contextMap.end(); ++map_it)
    {
        std::cout << "key=" << map_it->first << ": values[";
        finalDataString += "<Row>";
        for(vectorString::iterator it = map_it->second.begin(); it != map_it->second.end(); ++it) {
            std::cout << *it << ", ";
            finalDataString += "<Cell>";
            finalDataString += "<Data ss:Type=\"String\">";
            finalDataString += *it;
            finalDataString += "</Data>";
            finalDataString += "</Cell>";
        }
        std::cout << "]\n";
        finalDataString += "</Row>";
        finalDataString += "\n";
    }
    

    finalDataString += "</Table>";
    finalDataString += "\n";
    finalDataString += "</Worksheet>";
    finalDataString += "\n";
    
    
    
    finalDataString += "</Workbook>";
    
    
    std::ofstream outfile ("test.xls");
    outfile << finalDataString << '\n';
    outfile.close();
    

}
