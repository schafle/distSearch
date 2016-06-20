#define NOMINMAX

#include "targetver.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "LuceneHeaders.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

using namespace Lucene;

/// This demonstrates a typical paging search scenario, where the search engine presents pages of size n
/// to the user. The user can then go to the next page if interested in the next hits.
///
/// When the query is executed for the first time, then only enough results are collected to fill 5 result
/// pages. If the user wants to page beyond this limit, then the query is executed another time and all
/// hits are collected.
static void doPagingSearch(const SearcherPtr& searcher, const QueryPtr& query, int32_t hitsPerPage, bool raw, bool interactive) {
	// Collect enough docs to show 5 pages
	TopScoreDocCollectorPtr collector = TopScoreDocCollector::create(5 * hitsPerPage, false);
	searcher->search(query, collector);
	Collection<ScoreDocPtr> hits = collector->topDocs()->scoreDocs;

	int32_t numTotalHits = collector->getTotalHits();
	std::wcout << numTotalHits << L" total matching documents\n";

	int32_t start = 0;
	int32_t end = std::min(numTotalHits, hitsPerPage);

	//while (true) {
		if (end > hits.size()) {
			std::wcout << L"Only results 1 - " << hits.size() << L" of " << numTotalHits << L" total matching documents collected.\n";
			std::wcout << L"Collect more (y/n) ?";
			String line;
			std::wcin >> line;
			boost::trim(line);

			if (line.empty() || boost::starts_with(line, L"n")) {
			//	break;
			}

			collector = TopScoreDocCollector::create(numTotalHits, false);
			searcher->search(query, collector);
			hits = collector->topDocs()->scoreDocs;
		}

		end = std::min(hits.size(), start + hitsPerPage);

		for (int32_t i = start; i < end; ++i) {
			if (raw) { // output raw format
				std::wcout << L"doc=" << hits[i]->doc << L" score=" << hits[i]->score << L"\n";
				continue;
			}

			DocumentPtr doc = searcher->doc(hits[i]->doc);
			String path = doc->get(L"path");
			if (!path.empty()) {
				std::wcout << StringUtils::toString(i + 1) + L". " << path << L"\n";
				String title = doc->get(L"title");
				if (!title.empty()) {
					std::wcout << L"   Title: " << doc->get(L"title") << L"\n";
				}
			} else {
				std::wcout << StringUtils::toString(i + 1) + L". No path for this document\n";
			}
		}

		if (!interactive) {
		//	break;
		}

		/* if (numTotalHits >= end) {
			bool quit = false;
			while (true) {
				std::wcout << L"Press ";
				if (start - hitsPerPage >= 0) {
					std::wcout << L"(p)revious page, ";
				}
				if (start + hitsPerPage < numTotalHits) {
					std::wcout << L"(n)ext page, ";
				}
				std::wcout << L"(q)uit or enter number to jump to a page: ";

				String line;
				std::wcin >> line;
				boost::trim(line);

				if (line.empty() || boost::starts_with(line, L"q")) {
					quit = true;
					break;
				}
				if (boost::starts_with(line, L"p")) {
					start = std::max((int32_t)0, start - hitsPerPage);
					break;
				} else if (boost::starts_with(line, L"n")) {
					if (start + hitsPerPage < numTotalHits) {
						start += hitsPerPage;
					}
					break;
				} else {
					int32_t page = 0;
					try {
						page = StringUtils::toInt(line);
					} catch (NumberFormatException&) {
					}
					if ((page - 1) * hitsPerPage < numTotalHits) {
						start = std::max((int32_t)0, (page - 1) * hitsPerPage);
						break;
					} else {
						std::wcout << L"No such page\n";
					}
				}
			}
			if (quit) {
				break;
			}
			end = std::min(numTotalHits, start + hitsPerPage);
		}*/
	//}
}
