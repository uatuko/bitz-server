/*
 *	bitz-server, An ICAP server implementation in C++
 *	Copyright (C) 2012	Uditha Atukorala
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef BITZ_LOGGER_H
#define BITZ_LOGGER_H

#include <string>
#include <log4cpp/Category.hh>

namespace bitz {

	class Logger {
	public:
		static Logger &instance( std::string log_file = "/dev/null" , std::string category = "logger" ) {
			static Logger logger( log_file, category );
			return logger;
		}

		void initialise( std::string log_file, std::string category );

		static int getPriorityValue( const std::string& priority );
		void log( int priority, const std::string &message );

		void fatal( const std::string& message );
		void emerg( const std::string& message );
		void alert( const std::string& message );
		void crit( const std::string& message );
		void error( const std::string& message );
		void warn( const std::string& message );
		void notice( const std::string& message );
		void info( const std::string& message );
		void debug( const std::string& message );

	private:
		log4cpp::Category * LOGGER;

		Logger( std::string log_file, std::string category );
		~Logger();
		Logger( Logger const &copy );
		Logger &operator=( const Logger &copy );
	};

} /* end of namespace bitz */

#endif /* !BITZ_LOGGER_H */

