#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
using namespace std;

class CIniFile
{
public:
	struct Record
	{
		string Section;
		string Key;
		string Value;
	};

	enum CommentChar
	{
		Pound = '#',
		SemiColon = ';'
	};

	CIniFile(void);
	virtual ~CIniFile(void);

	static bool AddSection(string SectionName, string FileName);
	static bool Create(string FileName);
	static bool DeleteSection(string SectionName, string FileName);
	static string GetValue(string KeyName, string SectionName, string FileName);
	static bool SectionExists(string SectionName, string FileName);	
	static bool SetValue(string KeyName, string Value, string SectionName, string FileName);	
	static bool FileExists(string strFileName);//Added

private:

	static vector<Record> GetSections(string FileName);
	static vector<Record> GetRecord(string KeyName, string SectionName, string FileName);
	static bool RecordExists(string KeyName, string SectionName, string FileName);
	static bool Load(string FileName, vector<Record>& content);	
	static bool Save(string FileName, vector<Record>& content);

	struct RecordSectionIs : std::unary_function<Record, bool>
	{
		std::string section_;

		RecordSectionIs(const std::string& section): section_(section){}

		bool operator()( const Record& rec ) const
		{
			return rec.Section == section_;
		}
	};

	struct RecordSectionKeyIs : std::unary_function<Record, bool>
	{
		std::string section_;
		std::string key_;

		RecordSectionKeyIs(const std::string& section, const std::string& key): section_(section),key_(key){}

		bool operator()( const Record& rec ) const
		{
			return ((rec.Section == section_)&&(rec.Key == key_));
		}
	};
};