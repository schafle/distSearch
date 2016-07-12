#include "doPagingSearch.h"
#include "easylogging++.h"
using namespace Lucene;

/// This demonstrates a typical paging search scenario, where the search engine presents pages of size n
/// to the user. The user can then go to the next page if interested in the next hits.
///
/// When the query is executed for the first time, then only enough results are collected to fill 5 result
/// pages. If the user wants to page beyond this limit, then the query is executed another time and all
/// hits are collected.
//Original definition
//int doPagingSearch(const SearcherPtr& searcher, const QueryPtr& query, int32_t hitsPerPage, bool raw, std::string uuid) {

//Modified definition
std::string doPagingSearch(const SearcherPtr& searcher, const QueryPtr& query, int32_t hitsPerPage, std::string uuid) {
	// Collect enough docs to show 5 pages
	TopScoreDocCollectorPtr collector = TopScoreDocCollector::create(hitsPerPage, false);
	searcher->search(query, collector);
	Collection<ScoreDocPtr> hits = collector->topDocs()->scoreDocs;
	
	std::string searchResults="";
	int32_t numTotalHits = collector->getTotalHits();
	//std::wcout << numTotalHits << L" total matching documents\n";

	LOG(INFO) << uuid << " Total document found: " << numTotalHits; 

	LOG(INFO) << uuid << " Total matching documents collected " << hits.size() << " of " << numTotalHits;
	
	int32_t start = 0;
	int32_t end = std::min(hits.size(), hitsPerPage);

	for (int32_t i = start; i < end; ++i) {
		LOG(INFO) << uuid << " doc=" << hits[i]->doc << " score=" << hits[i]->score ;

		DocumentPtr doc = searcher->doc(hits[i]->doc);
		String path = doc->get(L"path");
		if (!path.empty()) {
			LOG(INFO) << uuid << " "<< StringUtils::toUTF8(path).c_str() << std::endl;
			searchResults += StringUtils::toUTF8(path).c_str();
			//Not interested in title at the moment
			//String title = doc->get(L"title");
			//if (!title.empty()) {
			//	std::wcout << L"   Title: " << doc->get(L"title") << L"\n";
			//}
		}
		else {
			LOG(DEBUG) << uuid << " "<<StringUtils::toString(i + 1) + L". No path for this document\n";
			//std::wcout << StringUtils::toString(i + 1) + L". No path for this document\n";
		}
	}
return searchResults;
}
