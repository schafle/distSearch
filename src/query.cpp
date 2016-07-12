/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#define NOMINMAX


#include <stdio.h>
#include <stdlib.h>

#include "tcpacceptor.h"

#include "targetver.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "LuceneHeaders.h"
#include "FilterIndexReader.h"
#include "OneNormsReader.h"
#include "MiscUtils.h"
#include "query.h"

using namespace Lucene;


class StreamingHitCollector : public Collector {
	public:
		StreamingHitCollector() {
			docBase = 0;
		}

		virtual ~StreamingHitCollector() {
		}

	protected:
		ScorerPtr scorer;
		int32_t docBase;

	public:
		/// simply print docId and score of every matching document
		virtual void collect(int32_t doc) {
			std::wcout << L"doc=" << (doc + docBase) << L" score=" << scorer->score();
		}

		virtual bool acceptsDocsOutOfOrder() {
			return true;
		}

		virtual void setNextReader(const IndexReaderPtr& reader, int32_t docBase) {
			this->docBase = docBase;
		}

		virtual void setScorer(const ScorerPtr& scorer) {
			this->scorer = scorer;
		}
};

/// This method uses a custom HitCollector implementation which simply prints out the docId and score of
/// every matching document.
///
/// This simulates the streaming search use case, where all hits are supposed to be processed, regardless
/// of their relevance.
static void doStreamingSearch(const SearcherPtr& searcher, const QueryPtr& query) {
	searcher->search(query, newLucene<StreamingHitCollector>());
}

int run_query(std::string index_location, std::string line, int hpp) { //hpp ==  hits per page
	/*if (argc == 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
		std::wcout << L"Usage: searchfiles.exe [-index dir] [-field f] [-repeat n] [-queries file] [-raw] ";
		std::wcout << L"[-norms field] [-paging hitsPerPage]\n\n";
		std::wcout << L"Specify 'false' for hitsPerPage to use streaming instead of paging search.\n";
		return 1;
	}*/
	
	int doc_found;
	try {
		String index = L"index";
		String field = L"contents";
		String queries;
		int32_t repeat = 0;
		bool raw = false;
		String normsField;
		bool paging = true;
		int32_t hitsPerPage = 10;

		index = StringUtils::toUnicode(index_location.c_str());
		hitsPerPage = hpp;
		/* for (int32_t i = 0; i < argc; ++i) {
			if (strcmp(argv[i], "-index") == 0) {
				index = StringUtils::toUnicode(argv[i + 1]);
				++i;
			} else if (strcmp(argv[i], "-field") == 0) {
				field = StringUtils::toUnicode(argv[i + 1]);
				++i;
			} else if (strcmp(argv[i], "-queries") == 0) {
				queries = StringUtils::toUnicode(argv[i + 1]);
				++i;
			} else if (strcmp(argv[i], "-repeat") == 0) {
				repeat = StringUtils::toInt(StringUtils::toUnicode(argv[i + 1]));
				++i;
			} else if (strcmp(argv[i], "-raw") == 0) {
				raw = true;
			} else if (strcmp(argv[i], "-norms") == 0) {
				normsField = StringUtils::toUnicode(argv[i + 1]);
				++i;
			} else if (strcmp(argv[i], "-paging") == 0) {
				if (strcmp(argv[i + 1], "false") == 0) {
					paging = false;
				} else {
					hitsPerPage = StringUtils::toInt(StringUtils::toUnicode(argv[i + 1]));
					if (hitsPerPage == 0) {
						paging = false;
					}
				}
				++i;
			}
		} */

		// only searching, so read-only=true
		IndexReaderPtr reader = IndexReader::open(FSDirectory::open(index), true);

		if (!normsField.empty()) {
			reader = newLucene<OneNormsReader>(reader, normsField);
		}

		SearcherPtr searcher = newLucene<IndexSearcher>(reader);
		AnalyzerPtr analyzer = newLucene<StandardAnalyzer>(LuceneVersion::LUCENE_CURRENT);
		QueryParserPtr parser = newLucene<QueryParser>(LuceneVersion::LUCENE_CURRENT, field, analyzer);

		ReaderPtr in;
		if (!queries.empty()) {
			in = newLucene<FileReader>(queries);
		}

		//while (true) {
		//String line;

		//if (!queries.empty()) {
		//	wchar_t c = in->read();
		//	while (c != L'\n' && c != L'\r' && c != Reader::READER_EOF) {
		//		line += c;
		//		c = in->read();
		//	}
		//} else {
		//std::wcout << L"Enter query: ";
		//getline(std::wcin, line); //Original
		//std::wcin >> line; //Working with this change; just to check if another string is accepted; stupid I know.
		//line = get_query(); //getting query from the tcpip connectopm
		//}
		//boost::trim(line);

		//if (line.empty()) {
		//break;
		//}

		std::wstring ws;
		ws.assign(line.begin(), line.end());
		std::wcout << ws << std::endl;
		QueryPtr query = parser->parse(ws);
		std::wcout << L"Searching for: " << query->toString(field) << L"\n";

		if (repeat > 0) { // repeat and time as benchmark
			int64_t start = MiscUtils::currentTimeMillis();
			for (int32_t i = 0; i < repeat; ++i) {
				searcher->search(query, FilterPtr(), 100);
			}
			std::wcout << L"Time: " << (MiscUtils::currentTimeMillis() - start) << L"ms\n";
		}

		if (paging) {
			doc_found = doPagingSearch(searcher, query, hitsPerPage, raw, queries.empty());
		} else {
			doStreamingSearch(searcher, query);
		}
		//}
		reader->close();
	} catch (LuceneException& e) {
		std::wcout << L"Exception: " << e.getError() << L"\n";
		return 1;
	}

	return doc_found;

}

