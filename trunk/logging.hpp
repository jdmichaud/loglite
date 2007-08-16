//  Boost general library logging.hpp header file  ---------------------------//

//  (C) Copyright Jean-Daniel Michaud 2007. Permission to copy, use, modify, 
//  sell and distribute this software is granted provided this copyright notice 
//  appears in all copies. This software is provided "as is" without express or 
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.
//  See http://www.boost.org/libs/logging/ for library home page.

#ifndef BOOST_LOGGING_HPP
#define BOOST_LOGGING_HPP

#include <stdio.h>
#include <string>
#include <ostream>
#include <sstream>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif
#if defined(BOOST_HAS_THREADS)
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#endif // BOOST_HAS_THREADS
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#define BOOST_LOG_INIT(format, ostream, max_log_level)		               \
{                                                                              \
  boost::logging::logger *l = boost::logging::logger::get_instance();          \
  boost::format f(format);						       \
  boost::sink s(ostream, max_log_level);				       \
  l->add_format(f);                                                            \
  l->add_sink(s, f);                                                           \
}

#define BOOST_LOG_ADD_OUTPUT_STREAM( stream, format )                          \
{                                                                              \
  boost::logging::logger *l = boost::logging::logger::get_instance();          \
  l->add_sink(stream, format);                                                 \
}

#define BOOST_LOG(level, _trace)                                               \
{                                                                              \
  boost::logging::logger *l = boost::logging::logger::get_instance();          \
  assert(l);                                                                   \
  if (l->get_max_log_level() >= level)                                         \
  {                                                                            \
    l->m_string_stream.str("");                                                \
    l->m_string_stream << _trace;                                              \
    l->trace(level, l->m_string_stream.str(), __FILE__, __LINE__);             \
  }                                                                            \
}

#define BOOST_LOG_UNFORMATTED(level, _trace)                                   \
{                                                                              \
  boost::logging::logger *l = boost::logging::logger::get_instance();          \
  assert(l);                                                                   \
  if (l->get_max_log_level() >= level)                                         \
  {                                                                            \
    l->m_string_stream.str("");                                                \
    l->m_string_stream << _trace;                                              \
    l->unformatted_trace(level, l->m_string_stream.str(), __FILE__, __LINE__); \
  }                                                                            \
}

#define BOOST_MAX_LINE_STR_SIZE 20 // log(2^64)
#define BOOST_LEVEL_UP_LIMIT    999

namespace boost {

  namespace logging {

//  Logging forward declarations ---------------------------------------------//
    class log_element;
    class level_element;
    class trace_element;
    class format;
    class sink;
    class logger;
    
//  Logging typedefs declarations --------------------------------------------//
    typedef enum { LEVEL = 0, TRACE, FILENAME, LINE }   param_e;
    typedef enum { SINK = 0, FORMAT }                   sink_format_assoc_e;
    typedef std::list<boost::shared_ptr<log_element> >  element_list_t;
    typedef std::list<boost::shared_ptr<std::ostream> > stream_list_t;
    typedef unsigned short                              level_t;
    typedef tuple<level_t, std::string, std::string, unsigned int> log_param_t;
    typedef std::list<format>                           format_list_t;
    typedef tuple<sink, format&>                        sink_format_assoc_t;
    typedef std::list<sink_format_assoc_t>             sink_format_assoc_list_t;

//  Used for shared_ptr() on statically allocated log_element ----------------//
    struct null_deleter
    { void operator()(void const *) const {} };

//  Logging classes declaration  ---------------------------------------------//
    class log_element
    {
    public:
      virtual std::string to_string() { assert(0); return ""; };

      virtual std::string visit(format &f, const log_param_t &log_param);
    };
    
    class level_element : public log_element
    {
    public:
      std::string to_string(level_t l) 
      { 
        return str(boost::format("%i") % l);
      };

      std::string visit(format &f, const log_param_t &log_param);
    };
    
    class filename_element : public log_element
    {
    public:
      std::string to_string(const std::string &f) { return f; }
      std::string visit(format &f, const log_param_t &log_param);
    };
    
    class line_element : public log_element
    {
    public:
      std::string to_string(unsigned int l) 
      {
        return str(boost::format("%i") % l);
      }
      std::string visit(format &f, const log_param_t &log_param);
    };
    
    class date_element : public log_element
    {
    public:
      std::string to_string()
      {
        boost::gregorian::date d(boost::gregorian::day_clock::local_day());
        return boost::gregorian::to_iso_extended_string(d);
      }
    };
    
    class time_element : public log_element
    {
    public:
      std::string to_string() 
      { 
        boost::posix_time::ptime 
          t(boost::posix_time::microsec_clock::local_time());
        return boost::posix_time::to_simple_string(t); 
      };
    };
    
    class trace_element : public log_element
    {
    public:
      std::string to_string(const std::string& s) { return s; };

      std::string visit(format &f, const log_param_t &log_param);
    };

    class eol_element : public log_element
    {
    public:
      std::string to_string() { return "\n"; };
    };

    class literal_element : public log_element
    {
    public:
      literal_element(const std::string &l) : m_literal(l) {}
      std::string to_string() { return m_literal; };
    private:
      std::string m_literal;
    };


//  Format class declatation -------------------------------------------------//
    class format
    {
    public:
      format(element_list_t e) 
        : m_element_list(e), m_identifier("unnamed") {}

      format(element_list_t e, const std::string &identifier) 
        : m_element_list(e), m_identifier(identifier) {}

      std::string produce_trace(const log_param_t &log_param)
      {
	element_list_t::iterator e_it = m_element_list.begin();
	std::stringstream str_stream;
	for (; e_it != m_element_list.end(); ++e_it)
        {
	  str_stream << (*e_it)->visit(*this, log_param);
	}

	return str_stream.str();
      }

      // Visitors for the log elements
      std::string accept(log_element &e)
      {
	return e.to_string();
      }
      std::string accept(level_element &e, level_t l)
      {
	return e.to_string(l);
      }
      std::string accept(trace_element &e, const std::string& s)
      {
	return e.to_string(s);
      }
      std::string accept(filename_element &e, const std::string& s)
      {
	return e.to_string(s);
      }
      std::string accept(line_element &e, unsigned int l)
      {
	return e.to_string(l);
      }

    private:
      element_list_t    m_element_list;
      std::string       m_identifier;
    };

//  Sink class declaration ---------------------------------------------------//
    class sink
    {
    public:
      sink(std::ostream *s, level_t max_log_level)
      {
        if (s)
          if (*s == std::cout || *s == std::cerr || *s == std::clog)
            m_output_stream = shared_ptr<std::ostream *>(s, null_deleter());
          else
            m_output_stream = shared_ptr<std::ostream *>(s);

        set_max_log_level(max_log_level);
      }

      void set_max_log_level(level_t max_log_level)
      { 
        m_max_log_level = ((BOOST_LEVEL_UP_LIMIT < max_log_level) 
          ? BOOST_LEVEL_UP_LIMIT : max_log_level);
      }

      inline level_t get_max_log_level() { return m_max_log_level; }

      void consume_trace(format &f, const log_param_t &log_param)
      {
	/* make here check to avoid producing a useless trace */
	if (get<LEVEL>(log_param) > m_max_log_level)
	  return;

	*m_output_stream << f.produce_trace(log_param);
      }

    private:
      level_t                  m_max_log_level;
      shared_ptr<std::ostream> m_output_stream;
    };
   
//  Element static instantiations --------------------------------------------//
    static level_element     level     = level_element();
    static filename_element  filename  = filename_element();
    static line_element      line      = line_element();
    static date_element      date      = date_element();
    static time_element      time      = time_element();
    static trace_element     trace     = trace_element();
    static eol_element       eol       = eol_element();

//  Logger class declaration  ------------------------------------------------//
    class logger
    {
    public: 
      logger() {}
        
      static logger *get_instance()
      {
#if defined(BOOST_HAS_THREADS)
	static boost::mutex m_inst_mutex;
	boost::mutex::scoped_lock scoped_lock(m_inst_mutex);
#endif // BOOST_HAS_THREADS
	static logger             *l = NULL;
        
	if (!l)
        {
	  l = new logger();
	  static shared_ptr<logger> s_ptr_l(l);
	}
          
	return l;
      }

      void add_format(const format &f)
      {
	m_format_list.push_back(f);
      }

      void add_sink(const sink &s)
      {
	m_sink_format_assoc.push_back
	  (
	   sink_format_assoc_t(s, *m_format_list.begin())
	  );
      }

      void add_sink(const sink &s, format &f)
      {
	m_sink_format_assoc.push_back(sink_format_assoc_t(s, f));
      }
           
      void trace(unsigned short     l, 
		 const std::string &t, 
		 const std::string &f, 
		 unsigned int      ln)
      {
#if defined(BOOST_HAS_THREADS)
	boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS

	log_param_t log_param(l, t, f, ln);
	sink_format_assoc_list_t::iterator 
	  s_it = m_sink_format_assoc.begin();
	for (; s_it != m_sink_format_assoc.end(); ++s_it)
        {
	  get<SINK>(*s_it).consume_trace(get<FORMAT>(*s_it), log_param);
	}
      }
      
      void unformatted_trace(unsigned short     l, 
			     const std::string &t, 
			     const std::string &f, 
			     unsigned int      ln);

    public:
      std::stringstream m_string_stream;
      
    private:
      format_list_t            m_format_list;
      sink_format_assoc_list_t m_sink_format_assoc;
#if defined(BOOST_HAS_THREADS)
      boost::mutex             m_mutex;
#endif // BOOST_HAS_THREADS
    };  // logger
    
//  Element functions definition ---------------------------------------------//
    inline std::string log_element::visit(format &f, 
                                          const log_param_t &log_param)
    {
      return f.accept(*this);
    }

    inline std::string level_element::visit(format &f, 
                                            const log_param_t &log_param)
    {
      return f.accept(*this, get<LEVEL>(log_param));
    }

    inline std::string trace_element::visit(format &f, 
                                            const log_param_t &log_param)
    {
      return f.accept(*this, get<TRACE>(log_param));
    }

    inline std::string filename_element::visit(format &f, 
                                               const log_param_t &log_param)
    {
      return f.accept(*this, get<FILENAME>(log_param));
    }

    inline std::string line_element::visit(format &f, 
                                           const log_param_t &log_param)
    {
      return f.accept(*this, get<LINE>(log_param));
    }

  } // !namespace logging

} // !namespace boost

//  Element global operators -------------------------------------------------//
inline boost::logging::element_list_t operator>>(
  boost::logging::log_element &lhs, 
  boost::logging::log_element &rhs)
{ 
  boost::logging::element_list_t l;
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&lhs, boost::logging::null_deleter()));
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return l;
}

inline boost::logging::element_list_t &operator>>(
  boost::logging::element_list_t &lhs, 
  boost::logging::log_element &rhs)
{ 
  lhs.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return lhs; 
}

inline boost::logging::element_list_t operator>>(
  const std::string &s, 
  boost::logging::log_element &rhs)
{
  boost::logging::element_list_t l;
  boost::shared_ptr<boost::logging::literal_element> 
    p(new boost::logging::literal_element(s));
  l.push_back(p);
  l.push_back(boost::shared_ptr<boost::logging::log_element> 
                (&rhs, boost::logging::null_deleter())); 
  return l;
}

inline boost::logging::element_list_t &operator>>(
  boost::logging::element_list_t &lhs, 
  const std::string &s)
{ 
  boost::shared_ptr<boost::logging::literal_element> 
    p(new boost::logging::literal_element(s));
  lhs.push_back(p);
  return lhs;
}

void boost::logging::logger::unformatted_trace(unsigned short     l, 
					       const std::string &t, 
					       const std::string &f, 
					       unsigned int      ln)
{
#if defined(BOOST_HAS_THREADS)
  boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS
  log_param_t log_param(l, t, f, ln);
  sink_format_assoc_list_t::iterator 
    s_it = m_sink_format_assoc.begin();
  for (; s_it != m_sink_format_assoc.end(); ++s_it)
    {
      boost::logging::format no_format(line >> eol);
      get<SINK>(*s_it).consume_trace(no_format, 
				     log_param);
    }
}

#endif  // !BOOST_LOGGING_HPP
