//Braydon Deroy
//April 5, 2015
//This program rports back file sizes and how many files are within the 
//type of video, music, or photo files.

#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace std::tr2::sys;

//Purpose: Add a vector up to create a sum and report it back to a vector value
//Parameters: A vector of any Mathmatical Data type T
//Returns: A sum of DataType T
template<typename T>
T Add(vector<T> v)
{
	T sum = 0;
	for (auto x : v)
		sum += x;
	return sum;
}

//Purpose: Swapping valuse of certain elements in any vector type
//Parameters: the Vector, and vector index at i, and k from for loop
//Returns: None
template<typename T>
void Swapping(vector<T>& v, int i, int k)
{
	T temp = v[i];
	v[i] = v[k];
	v[k] = temp;
}

//Purpose: To sort the summary alphabetically/reverse alphabetically, or by size
//Parameters: vector of required extensions, vector of extension sizes
			// vector of files found for one extension
			// boolean for sort by size
			// boolean for sorting by reverse alphabetical
//Returns: None;
void Sorting(vector<string> &ext, vector<long long> &extSize, vector<int> &report, bool sizeSort, bool alpRe)
{
	//sort by Size
	if (sizeSort)
	{
		for (int i = 0; i < extSize.size() - 1; ++i)
		{
			for (int k = i + 1; k < extSize.size(); ++k)
			{
				if (extSize[i] > extSize[k])
				{
					Swapping(ext, i, k);
					Swapping(extSize, i, k);
					Swapping(report, i, k);
				}
			}
		}
	}
	//Sort Reverse Alphabetically
	if (alpRe)
	{
		cout << "Im here" << endl;
		for (int i = 0; i < ext.size() - 1; ++i)
		{
			for (int k = i + 1; k < ext.size(); ++k)
			{
				//putting the extensions into two strings
				string letter1 = ext[i];
				string letter2 = ext[k];

				//getting the begining character of the two extensions
				//and checking the letter and putting them in reverse alphabetical order.
				char check1 = letter1[2];
				char check2 = letter2[2];
				if (check1 > check2)
				{
					Swapping(ext, i, k);
					Swapping(extSize, i, k);
					Swapping(report, i, k);
				}
			}
		}
	}
	//Sort Alphabetically
	if (!alpRe && !sizeSort)
	{
		for (int i = 0; i < ext.size() - 1; ++i)
		{
			for (int k = i + 1; k < ext.size(); ++k)
			{
				//putting the extensions into two strings
				string letter1 = ext[i];
				string letter2 = ext[k];

				//getting the begining character of the two extensions
				//and checking the letter and putting them in alphabetical order.
				char check1 = letter1[2];
				char check2 = letter2[2];
				if (check1 > check2)
				{
					Swapping(ext, i, k);
					Swapping(extSize, i, k);
					Swapping(report, i, k);
				}
			}
		}
	}
}

class Extensions
{
public:
	//music, video, and picture extensions
	vector<string> exten_;
	string regMatch_;

	//booleans for v, p, and m switches
	bool music_;
	bool photo_;
	bool video_;

	//no-arg constructor/defult constructor
	Extensions() {}

	//5-arg constrcutor
	Extensions(string reg, vector<string>& required, bool m, bool p, bool v) : regMatch_(reg), music_(m), photo_(p), video_(v)
	{
		//vector that has all the specified extensions
		exten_ = { "mp3", "wma", "wav", "mid", "jpg", "tiff", "bmp", "gif", "png", "avi", "mpeg", "mp4", "3gp", "wmv", "mkv" };

		//Method that constructs a extension required vector
		AddExten(exten_, regMatch_, required);
	}

	//Purpose: To Assign specific extensions into an empty vector
	//Parameters: Vector of strings, regex expression if any, and an empty vector of strings
	//Returns: A vector of strings
	vector<string> AddExten(vector<string>& exten, string& reg, vector<string>& required)
	{
		//checks to see if a regex expression was made or m, p, or v files made
		if (reg != "")
		{
			//CHecks each extension from the regex expression and compares it to each of the file 
			// extensions in the extension vector
			for (int i = 0; i < exten.size(); ++i)
			{
				int found = reg.find(exten[i]);
				if (found != -1)
					required.push_back("." + exten[i]);
			}
		}
		else
		{
			//goes through checks to make sure that 
			if (music_ && video_ && photo_)
				for (int i = 0; i < exten.size(); ++i)
					required.push_back("." + exten[i]);
			if (music_ && photo_ && !video_)
				for (int i = 0; i < 9; ++i)
					required.push_back("." + exten[i]);
			if (music_ && video_ && !photo_)
			{
				for (int i = 0; i < 4; ++i)
					required.push_back("." + exten[i]);
				for (int i = 9; i < exten.size(); ++i)
					required.push_back("." + exten[i]);
			}
			if (photo_ && video_ && !music_)
				for (int i = 4; i < exten.size(); ++i)
					required.push_back("." + exten[i]);
			if (music_ && !video_ && !photo_)
				for (int i = 0; i < 4; ++i)
					required.push_back("." + exten[i]);
			if (photo_ && !video_ && !music_)
				for (int i = 4; i < 9; ++i)
					required.push_back("." + exten[i]);
			if (video_ && !photo_ && !music_)
				for (int i = 9; i < exten.size(); ++i)
					required.push_back("." + exten[i]);
		}
		return required;
	}
};//end of class

//Purpose: Go through A curtain path and report back extensions recursivly
//Parameters: Path of the starting folder, vector of required extensions to find , 
//			vector<int> keep track of file size , a vector of maps to report the path of the extension
//Returns: Nothing
void Recursive(path const& folder, vector<string> extensions, vector<long long> &extenSize, vector< map<string, int>> & Report)
{
	directory_iterator d(folder);
	directory_iterator e;				// end of any folder

	for (; d != e; ++d)
	{
		for (int k = 0; k < extensions.size(); ++k)
		{
			int found = d->path().extension().find(extensions[k]);
			if (found != -1)
			{
				extenSize[k] += file_size(d->path());
				++Report[k][d->path().branch_path()];
			}
		}
		
		// if the item is  adirectory use recursion to parse that directory
		if (is_directory(d->status()))
			Recursive(d->path(), extensions, extenSize, Report);
	}
}


void Special(path const& folder, vector<string> extensions, vector<long long> &extenSize, vector< map<string, int>> & Report)
{
	directory_iterator d(folder);
	directory_iterator e;				// end of any folder
	for (; d != e; ++d)
	{
		// if the item is  adirectory use recursion to parse that directory
		if (d->path().extension() == "" && !is_directory(d->path()))
		{
			extenSize[0] += file_size(d->path());
			++Report[0][d->path().directory_string()];
		}
		if (is_directory(d->status()))
			Special(d->path(), extensions, extenSize, Report);
	}
}

//Purpose: Go through A curtain path and report back extensions non-recursivly
//Parameters: Path of the starting folder, vector of required extensions to find, 
//			  Empty vector of extenSize to store extension sizes,
//			  An empty report to fill in the required path fpor each extension found
//Returns: Nothing
void NonRecursive(path const& folder, vector<string> extensions, vector<long long> &extenSize, vector< map<string, int>> &Report)
{
	recursive_directory_iterator d(folder);		//creates a directory itorator that paths to folder
	recursive_directory_iterator e;	// end of any folder
	for (; d != e; ++d)
	{
		if (!is_directory(d->status()))
		{
			for (int k = 0; k < extensions.size(); ++k)
			{
				cout << " " << d->path().extension() << " " << endl;
				int found = d->path().extension().find(extensions[k]);
				if (found != -1)
				{
					extenSize[k] += file_size(d->path());
					++Report[k][d->path().branch_path()];
				}
			}
		}
		else
		{
			++d;
			for (int k = 0; k < extensions.size(); ++k)
			{
				cout << " " << d->path().extension() << " " << endl;
				int found = d->path().extension().find(extensions[k]);
				if (found != -1)
				{
					extenSize[k] += file_size(d->path());
					++Report[k][d->path().branch_path()];
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//Thousands seperator
	struct seperator : numpunct < char >
	{
	protected:
		virtual char do_thousands_sep() const override { return ','; }
		virtual string do_grouping() const override { return "\03"; }
	};
	cout.imbue(locale(cout.getloc(), new seperator));

	cout << "LocateMedia (C) 2015, Braydon Deroy" << endl;
	//checks if there is more than four arguments
	///////
	///////
	if (argc > 5)
	{
		cout << "Error: to many arguments specife with 4 or less arguments" << endl;
		return EXIT_FAILURE;
	}

	//Search for the Help switch if aplicable
	///////
	///////
	bool help = false;
	for (int i = 1; i < argc; ++i)
	{
		string helpCheck = (string)argv[i];
		for (int k = 0; k < helpCheck.length(); ++k)
		{
			char ch = helpCheck.at(k);
			if (ch == 'h')
				help = true;
		}

		if (help == true)
		{
			cout << "Usage: LocationMedia [-hnRds(mpv|x	[regularexpression])] [folder]" << endl;
			return EXIT_FAILURE;
		}
	}

	//Search for the placement of the switches
	//////
	//////
	int switchFound = 0;
	bool sw = false;
	bool xFound = false;
	for (int i = 1; i < argc; ++i)
	{
		string checkPlaceMent = (string)argv[i];
		if (sw == false)
		{
			for (int k = 0; k < checkPlaceMent.length(); ++k)
			{
				char check = checkPlaceMent.at(k);
				if (check == '-')
				{
					switchFound = i;
					sw = true;
				}

				if (check == 'x')
					xFound = true;

				if (xFound == true)
				{
					for (int f = 0; f < checkPlaceMent.length(); ++f)
						if (checkPlaceMent.at(f) == 'm' || checkPlaceMent.at(f) == 'p' || checkPlaceMent.at(f) == 'v')
						{
							cout << "Error: cannot combine x with any of the m, p, v switches" << endl;
							return EXIT_FAILURE;
						}
				}
			}
		}
		else
			break;
	}

	//Searching for the starting folder
	///////
	///////
	int folder = 0;
	bool foundFolder = false;
	for (int i = 1; i < argc; ++i)
	{
		string folderCheck = (string)argv[i];
		for (int k = 0; k < folderCheck.length(); ++k)
		{
			char ch = folderCheck.at(k);
			if (ch == '\\')
			{
				folder = i;
				foundFolder = true;
				break;
			}
		}
		if (foundFolder)
			break;
	}

	//folder that it is starting at.
	///////
	///////
	path StartFolder = ".";
	if (foundFolder)
		StartFolder = (string)argv[folder];

	//Checks to see if there is an argument for the x switch
	if (xFound && argc <= 3)
	{
		cerr << "Error: extension expression is missing." << endl;
		return EXIT_FAILURE;
	}

	//checking to see if folder is exsiting in computer
	///////
	///////
	if (!exists(StartFolder))
	{
		cerr << StartFolder << " file doesnn't exist in computer. PLease enter a valid file pathway" << endl;
		return EXIT_FAILURE;
	}

	//A regex expression if 'x' is provided
	////////
	////////
	string regMatch = "";
	bool regFound = false;
	if (sw && xFound)
	{
		regMatch = (string)argv[switchFound + 1];
		string test = (string)argv[folder];
		int check = regMatch.compare(test);
		if (check == 0)
		{
			cout << "Error: Bad Regular Expression" << "\n"
				<< "regex_error(error_escape): The expression contained an invalid escaped character, or a trailing escape." << endl;
			return EXIT_FAILURE;
		}
		else
			regFound = true;
	}

	//boolean statements for the switches
	////////
	////////
	bool musicFind = false;
	bool photoFind = false;
	bool videoFind = false;
	bool Reverse = false;
	bool sortSize = false;
	bool nonRecur = false;
	bool dir = false;

	//weather sortsize comes first or Reverse
	////////
	////////
	int soSi = 0;
	int rev = 0;

	//checking what kind of switches are present if switches are found
	////////
	////////
	string switches = (string)argv[switchFound];
	if (!sw)
		switches = "";
	if (sw == true)
	{
		bool onefile = false;
		//switches for m, v, and p files if no 'x' switche is provided
		///////
		///////
		if (!xFound && !regFound)
		{
			//////
			//////To check and see is there is a sepecified m, p, or v switch
			//////
			int place = 0;
			for (int f = 0; f < switches.length(); ++f)
			{
				char ch = switches.at(f);
				if (ch == '(')
				{
					place = f;
					onefile = true;
					break;
				}
			}

			if (onefile)
			{
				for (int k = place; k < switches.length(); ++k)
				{
					char ch = switches.at(k);
					if (ch == ')')
						break;
					else
					{
						switch (switches.at(k))
						{
						case 'm':
							musicFind = true;
							break;
						case 'p':
							photoFind = true;
							break;
						case 'v':
							videoFind = true;
							break;
						default:
							break;
						}
					}
				}
			}
		}

		//search for m, v, or p files if one file is not sepecified
		////////
		////////
		if (!onefile)
		{
			for (int i = 0; i < switches.length(); ++i)
			{
				switch (switches.at(i))
				{
				case 'm':
					musicFind = true;
					break;
				case 'p':
					photoFind = true;
					break;
				case 'v':
					videoFind = true;
					break;
				default:
					break;
				}
			}
		}

		//switches for directory, sorting byreverse order, non-recursive, amd sorting by size
		//and serch file switches if aplicable 
		////////
		for (int i = 0; i < switches.length(); ++i)
		{
			switch (switches.at(i))
			{
			case 'd':
				dir = true;
				break;
			case 'R':
				Reverse = true;
				rev = i;
				break;
			case 'N':
				nonRecur = true;
				break;
			case 's':
				sortSize = true;
				soSi = i;
				break;
			default:
				break;
			}
		}
	}

	// if sort and reverse are both turned on
	///////
	///////
	if (sortSize == true && Reverse == true)
	{
		if (soSi < rev)
			Reverse = false;
		else if (soSi > rev)
			sortSize = false;
	}

	/////	Defult setting
	bool defSetting = false;
	if (sw == false && foundFolder == false)
	{
		defSetting = true;
		musicFind = true;
		photoFind = true;
		videoFind = true;
	}

	//An empty vector to fill the required extensions
	vector<string> requiredEx;

	//To check and see if regex was expressed and m, v, or p switches where stated
	if (regMatch != "" && (musicFind || photoFind || videoFind))
	{
		regMatch = "";
		musicFind = false;
		photoFind = false;
		videoFind = false;
	}

	/////	sending booleans v, m, or p to Expression class for file extraction and 
	/////	passing the extensions vector to fill in the required extensions that
	/////	are given with the regex expression
	Extensions(regMatch, requiredEx, musicFind, photoFind, videoFind);

	/////	Sets the size of the extension size vector to the 
	/////	size of the reuired extension vector
	vector<long long> extensionSize(requiredEx.size());

	//A vector of collections that will display the directory report
	vector< map<string, int> > reports(requiredEx.size());

	bool speCase = false;
	if (xFound && regFound)
	{
		speCase = true;
		requiredEx.resize(1);
		extensionSize.resize(1);
		reports.resize(1);
	}
	//Go through the files recursivly, non-recursivly, or by default
	//map<string, int> temp;
	if (defSetting)
		Recursive(StartFolder, requiredEx, extensionSize, reports);
	else
	{
		if (!speCase)
		{
			if (nonRecur)
				NonRecursive(StartFolder, requiredEx, extensionSize, reports);
			else
				Recursive(StartFolder, requiredEx, extensionSize, reports);
		}
		else
			Special(StartFolder, requiredEx, extensionSize, reports);
	}

	//Create three vectors that will store actual data
	vector<string> repExt;
	vector<long long> repExtSiz;
	vector<map<string, int>> reploc;
	//fill in the actual non-null data to empty replacements
	for (int i = 0; i < extensionSize.size(); ++i)
	{
		if (extensionSize[i] != 0)
		{
			repExt.push_back(requiredEx[i]);
			repExtSiz.push_back(extensionSize[i]);
			reploc.push_back(reports[i]);
		}
	}

	//print the report if sepcified
	if (dir)
	{
		cout << "\nDetail Report" << endl;
		cout << "-------------\n" << endl;
	}

	map<string, int> temp;
	map<string, int>::iterator it;
	vector<int> fileSum(reploc.size());
	for (int i = 0; i < reploc.size(); ++i)
	{
		int sum = 0;
		temp = reploc[i];
		for (it = temp.begin(); it != temp.end(); ++it)
			sum += it->second;
		fileSum[i] = sum;

		//print out specified folders if specified
		if (dir)
		{
			cout << repExt[i] << " (" << temp.size() << " folders)" << endl;
			for (it = temp.begin(); it != temp.end(); ++it)
				cout << setw(8) << it->second << ":" << setw(2) << "\t" << it->first << endl;
			cout << endl;
		}
	}

	//outputting the start of the summary
	cout << "Summary: " << StartFolder << endl << "\n";
	cout << setw(5) << "Ext" << setw(2) << ":" << setw(8) << "#" << setw(2) << ":" << setw(14) << "Total" << endl;
	cout << "-----" << setw(2) << ":" << setw(8) << "-------" << setw(2) << ":" << setw(14) << "-------------" << endl;

	//Sorting by alphabetical, reAlphabetical, or by size
	if (fileSum.size() != 0 && !defSetting)
		Sorting(repExt, repExtSiz, fileSum, sortSize, Reverse);

	//printing out the report
	for (int i = 0; i < repExt.size(); ++i)
	{
		
		cout << setw(5) << repExt[i] << setw(2) << ":" << setw(8) << fileSum[i] << setw(2) << ":" << setw(14) << repExtSiz[i] << endl;
	}

	cout << "-----" << setw(2) << ":" << setw(8) << "-------" << setw(2) << ":" << setw(14) << "-------------" << endl;
	cout << setw(5) << repExt.size() << setw(2) << ":" << setw(8) << Add(fileSum) << setw(2) << ":" << setw(14) << Add(repExtSiz) << endl << "\n";
}