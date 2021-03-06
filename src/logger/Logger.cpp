/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * \file Logger.cpp
 * \brief Contains definition of logger methods.
 * \author tkornuta
 * \date Dec 1, 2015
 */

#include <logger/Logger.hpp>

#include <boost/foreach.hpp>

namespace mic {
namespace logger {

// Init application instance - as NULL.
boost::atomic<Logger*> Logger::instance_(NULL);

// Initilize mutex.
boost::mutex Logger::instantiation_mutex;


Logger* Logger::getInstance() {
	// Try to load the instance - first check.
	Logger* tmp = instance_.load(boost::memory_order_consume);
	// If instance does not exist.
	if (!tmp) {
		// Enter critical section.
		boost::mutex::scoped_lock guard(instantiation_mutex);
		// Try to load the instance - second check.
		tmp = instance_.load(boost::memory_order_consume);
		// If still does not exist - create new instance.
		if (!tmp) {
			tmp = new Logger;
			instance_.store(tmp, boost::memory_order_release);
		}//: if
		// Exit critical section.
	}//: if
	// Return instance.
	return tmp;
}


Logger::Logger() {
}


void Logger::addOutput(LoggerOutput * out) {
	outputs.push_back(out);
}


void Logger::log(const std::string & file, int line, Severity_t sev, const std::string & msg) {
	BOOST_FOREACH(LoggerOutput & output, outputs) {
		if (sev < output.getLvl())
			continue;

		output.print(msg, sev, file, line);
	}
}



void Logger::incrementSeverityLevel() {
	BOOST_FOREACH(LoggerOutput & output, outputs)
		output.incrementLvl();
}

void Logger::decrementSeverityLevel() {
	BOOST_FOREACH(LoggerOutput & output, outputs)
		output.decrementLvl();
}


void Logger::setSeverityLevel(Severity_t sev) {
	BOOST_FOREACH(LoggerOutput & output, outputs)
		output.setLvl(sev);
}


} /* namespace logger */
} /* namespace mic */

