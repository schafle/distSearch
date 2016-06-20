/// Use the norms from one field for all fields.  Norms are read into memory, using a byte of memory
/////// per document per searched field.  This can cause search of large collections with a large number
/////// of fields to run out of memory.  If all of the fields contain only a single token, then the norms
/////// are all identical, then single norm vector may be shared.

#define NOMINMAX

#include "targetver.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "LuceneHeaders.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

using namespace Lucene;

class OneNormsReader : public FilterIndexReader {
	public:
		OneNormsReader(const IndexReaderPtr& in, const String& field) : FilterIndexReader(in) {
			this->field = field;
		}   

		virtual ~OneNormsReader() {
		}

	protected:
		String field;

	public:
		virtual ByteArray norms(const String& field) {
			return in->norms(this->field);
		}   
};

