//  Boost general library logging.hpp header file  ---------------------------/

//  (C) Copyright Jean-Daniel Michaud, Ilya Pokolev 2007. 
//  Permission to copy, use, modify, sell and distribute this software is 
//  granted provided this copyright notice appears in all copies. 
//  This software is provided "as is" without express or implied warranty, 
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/LICENSE_1_0.txt for licensing.
//  See http://code.google.com/p/loglite/ for library home page.

#ifndef BOOST_LOGGING_HPP
#define BOOST_LOGGING_HPP

#include <list>
#include <stack>
#include <string>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <exception>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif
#if defined(BOOST_HAS_THREADS)
#  include <boost/thread/once.hpp>
#  include <boost/thread/thread.hpp>
#  include <boost/thread/condition.hpp>
#endif // BOOST_HAS_THREADS
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

// Code copied from redhat website
#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif
// !Code copied from redhat website

#ifndef BOOST_NO_CODE_GENERATION_FOR_LOG
#define BOOST_LOG_INIT( format )                                              \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  l->add_format(format);                                                      \
}

#define BOOST_LOG_ADD_OUTPUT_STREAM( sink )                                   \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  l->add_sink(sink);                                                          \
}

#define BOOST_LOG(mask, qualifier, _trace)                                    \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  if (l->get_global_log_mask() & mask)                                        \
  {                                                                           \
    std::stringstream string_stream;                                          \
    string_stream << _trace;                                                  \
    boost::logging::log_param_t log_param = { mask,                           \
                              &qualifier,                                     \
                              string_stream.str(),                            \
                              __FILE__, __LINE__,                             \
                              __func__, __PRETTY_FUNCTION__ };                \
    l->trace(log_param);                                                      \
  }                                                                           \
}

#define BOOST_LOG_( mask, _trace )                                            \
  { BOOST_LOG(mask, boost::logging::log, _trace) }

#define BOOST_LOG_UNFORMATTED(mask, qualifier, _trace)                        \
{                                                                             \
  boost::logging::logger_p l = boost::logging::logger::get_instance();        \
  if (l->get_global_log_mask() & mask)                                        \
  {                                                                           \
    std::stringstream string_stream;                                          \
    string_stream << _trace;                                                  \
    boost::logging::log_param_t log_param = { mask,                           \
                              &qualifier,                                     \
                              string_stream.str(),                            \
                              __FILE__, __LINE__,                             \
                              __func__, __PRETTY_FUNCTION__ } ;               \
    l->unformatted_trace(log_param);                                          \
  }                                                                           \
}

#else // !BOOST_NO_CODE_GENERATION_FOR_LOG
#define BOOST_LOG_INIT( format )
#define BOOST_LOG_ADD_OUTPUT_STREAM( sink )
#define BOOST_LOG(mask, qualifier, _trace)
#define BOOST_LOG_( mask, _trace )
#define BOOST_LOG_UNFORMATTED(mask, qualifier, _trace)
#endif // BOOST_NO_CODE_GENERATION_FOR_LOG

#define BOOST_LOG_MAX_LINE_STR_SIZE       20    // log(2^64)
#define BOOST_LOG_MASK_UP_LIMIT           65535 // 2^sizeof (mask_t) - 1
#define BOOST_LOG_MASK_LEVEL_1            1     // 0001
#define BOOST_LOG_MASK_LEVEL_2            3     // 0011
#define BOOST_LOG_MASK_LEVEL_3            7     // 0111
#define BOOST_LOG_MASK_LEVEL_4            15    // 1111
#define BOOST_LOG_LEVEL_1                 1
#define BOOST_LOG_LEVEL_2                 2
#define BOOST_LOG_LEVEL_3                 4
#define BOOST_LOG_LEVEL_4                 8

#define BOOST_LOG_L1( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_1, _trace )
#define BOOST_LOG_L2( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_2, _trace )
#define BOOST_LOG_L3( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_3, _trace )
#define BOOST_LOG_L4( _trace )  BOOST_LOG_( BOOST_LOG_LEVEL_4, _trace )

namespace boost {

  namespace logging {

//  Logging forward declarations ---------------------------------------------//
    class log_element;
    class mask_element;
    class qualifier;
    class trace_element;
    class format;
    class sink;
    class logger;
    
//  Logging typedefs declarations --------------------------------------------//
    typedef enum { SINK = 0, FORMAT }                   sink_format_assoc_e;
    typedef std::list<boost::shared_ptr<log_element> >  element_list_t;
    typedef std::list<boost::shared_ptr<std::ostream> > stream_list_t;
    typedef unsigned short                              mask_t;
    typedef struct
    {
      mask_t           m_mask;
      const qualifier *m_qualifier;
      std::string      m_trace;
      std::string      m_filename;
      unsigned int     m_line;
      std::string      m_func_name;
      std::string      m_func_sig;
    } log_param_t;
    typedef std::list<format>                           format_list_t;
    typedef tuple<sink, format>                         sink_format_assoc_t;
    typedef std::list<sink_format_assoc_t>            sink_format_assoc_list_t;
    typedef std::list<qualifier *>                      qualifier_list_t;
    typedef shared_ptr<logger>                          logger_p;

//  Used for shared_ptr() on statically allocated log_element ----------------//
    struct null_deleter
    { void operator()(void const *) const {} };

//  Qualifier class declaration ----------------------------------------------//
    class qualifier
    {
    public:
      qualifier() {}
      inline std::string to_string() const { return m_identifier; }
      virtual bool operator==(const qualifier &q) { return false; }
    protected:
      std::string m_identifier;
    };

    class log_qualifier : public qualifier 
    {
    public:
      log_qualifier() { m_identifier = "log"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const log_qualifier *>(&q) != NULL); }
    };

    class notice_qualifier : public qualifier
    {
    public:
      notice_qualifier() { m_identifier = "notice"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const notice_qualifier *>(&q) != NULL); }
    };

    class warning_qualifier : public qualifier
    {
    public:
      warning_qualifier() { m_identifier = "warning"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const warning_qualifier *>(&q) != NULL); }
    };

    class error_qualifier : public qualifier
    {
    public:
      error_qualifier() { m_identifier = "error"; }
      bool operator==(const qualifier &q) 
      { return (dynamic_cast<const error_qualifier *>(&q) != NULL); }
    };

//  Element classes declaration  ---------------------------------------------//
    class log_element
    {
    public:
      virtual std::string to_string() { assert(0); return ""; };

      virtual std::string visit(format &f, const log_param_t &log_param);
    };
    
    class mask_element : public log_element
    {
    public:
      std::string to_string(mask_t l) 
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
      explicit literal_element(std::string l) : m_literal(l) {}
      std::string to_string() { return m_literal; };
    private:
      std::string m_literal;
    };

    class qualifier_element : public log_element
    {
    public:
      qualifier_element(const qualifier &lq) 
      {
        m_qualifier_identifier = lq.to_string();
      }
      std::string to_string() { return m_qualifier_identifier; };
    private:
      std::string m_qualifier_identifier;
    };

    class eot_element : public log_element  //end-of-trace qualifier
    {
    public:
      std::string to_string() { return "\f"; };
    };

    class function_name_element : public log_element  //function name qualifier
    {
    public:
      std::string to_string(const std::string &n) { return n; }
      std::string visit(format &f, const log_param_t &log_param);
    };

    // pretty function name qualifier
    class pretty_function_name_element : public log_element
    {
    public:
      std::string to_string(const std::string &n) { return n; }
      std::string visit(format &f, const log_param_t &log_param);
    };

//  Format class declatation ------------------------------------------------//
    class format
    {
    public:
      format(log_element &e)
        : m_identifier("unnamed") 
      {
        boost::shared_ptr<boost::logging::log_element> p(&e, null_deleter());
        m_element_list.push_back(p);
      }

      format(log_element &e, const std::string &identifier) 
        : m_identifier(identifier) 
      {
        boost::shared_ptr<boost::logging::log_element> p(&e, null_deleter());
        m_element_list.push_back(p);
      }

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
      std::string accept(mask_element &e, mask_t l)
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
      std::string accept(function_name_element &e, const std::string& s)
      {
        return e.to_string(s);
      }
      std::string accept(pretty_function_name_element &e, const std::string& s)
      {
        return e.to_string(s);
      }

    private:
      element_list_t    m_element_list;
      std::string       m_identifier;
    };

//  Sink class declaration --------------------------------------------------//
    class sink
    {
    public:
      sink(std::ostream *s, mask_t log_mask = 1)
      {
        if (s)
          if (*s == std::cout || *s == std::cerr || *s == std::clog)
            m_output_stream.reset(s, null_deleter());
          else
            m_output_stream.reset(s);

        set_log_mask(log_mask);
      }
      
      void set_log_mask(mask_t log_mask)
      { 
        //        m_log_mask = ((BOOST_LOG_MASK_UP_LIMIT < log_mask) 
        //          ? BOOST_LOG_MASK_UP_LIMIT : log_mask);
        m_log_mask = log_mask;
      }

      inline mask_t get_log_mask() const { return m_log_mask; }

      void consume_trace(format &f, const log_param_t &log_param)
      {
        /* make here check to avoid producing a useless trace */
        if (log_param.m_mask > m_log_mask)
          return ;

        qualifier_list_t::const_iterator it = m_qualifier_list.begin();
        bool qualifier_present = false;
        for ( ; !qualifier_present && it !=  m_qualifier_list.end(); ++it)
          qualifier_present = (**it == *log_param.m_qualifier);

        if (!qualifier_present)
          return ;

        *m_output_stream << f.produce_trace(log_param);
      }

      void attach_qualifier(qualifier &q)
      {
        m_qualifier_list.push_back(&q);
      }

    private:
      mask_t                   m_log_mask;
      shared_ptr<std::ostream> m_output_stream;
      qualifier_list_t         m_qualifier_list;      
    };
   
//  Element static instantiations --------------------------------------------//
    static mask_element      mask      = mask_element();
    static filename_element  filename  = filename_element();
    static line_element      line      = line_element();
    static date_element      date      = date_element();
    static time_element      time      = time_element();
    static trace_element     trace     = trace_element();
    static eol_element       eol       = eol_element();
    static eot_element       eot       = eot_element();
    static function_name_element 
                       function_name   = function_name_element();
    static pretty_function_name_element 
                  function_signature   = pretty_function_name_element();

    static log_qualifier     log       = log_qualifier();
    static notice_qualifier  notice    = notice_qualifier();
    static warning_qualifier warning   = warning_qualifier();
    static error_qualifier   error     = error_qualifier();

//  Logger class declaration  ------------------------------------------------//
    static logger_p g__logger;
    class logger
    {
    public: 
      logger() : m_global_log_mask(0) {}
        
      static void create_instance()
      {
        g__logger.reset(new logger());
      }

      static logger_p &get_instance()
      {
        static boost::once_flag   once = BOOST_ONCE_INIT;
        boost::call_once(logger::create_instance, once);
        return g__logger;
      }

      void add_format(format f)
      {
        m_format_list.push_back(f);
      }

      void add_sink(sink s)
      {
        if (m_format_list.begin() == m_format_list.end())
          throw "no format defined";

        // Updating global_mask used for full lazy evaluation
        m_global_log_mask |= s.get_log_mask();

        m_sink_format_assoc.push_back
          (
            sink_format_assoc_t(s, *m_format_list.begin())
          );
      }

      void add_sink(sink s, format f)
      {
        // Updating global_mask used for full lazy evaluation
        m_global_log_mask |= s.get_log_mask();

        m_sink_format_assoc.push_back(sink_format_assoc_t(s, f));
      }

      inline mask_t get_global_log_mask() 
      { return m_global_log_mask; }

      void trace(mask_t m, 
                 const qualifier &q, 
                 const std::string &s, 
                 const std::string &f = "<unknown>",
                 unsigned int l = 0)
      {
        log_param_t log_param = { m, &q, s, f, l, "<unknown>", "<unknown>" };
        trace(log_param);
      }

      void unformatted_trace(mask_t m, 
                             const qualifier &q, 
                             const std::string &s)
      {
        log_param_t log_param = { m, &q, s, 
                                  "<unknown>", 0, "<unknown>", "<unknown>" };
        unformatted_trace(log_param);
      }

      void trace(const log_param_t &log_param)
      {
#if defined(BOOST_HAS_THREADS)
        boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS

        sink_format_assoc_list_t::iterator 
          s_it = m_sink_format_assoc.begin();
        for (; s_it != m_sink_format_assoc.end(); ++s_it)
        {
          get<SINK>(*s_it).consume_trace(get<FORMAT>(*s_it), log_param);
        }
      }
      
      void unformatted_trace(const log_param_t &log_param);

    private:
      format_list_t            m_format_list;
      sink_format_assoc_list_t m_sink_format_assoc;

      // The global log mask is the highest log mask on all the sink
      // added to the logger. If no sink as a log mask matching the global
      // mask for a trace, the trace does not need to be evaluated.
      mask_t                  m_global_log_mask;
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

    inline std::string mask_element::visit(format &f, 
                                           const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_mask);
    }

    inline std::string trace_element::visit(format &f, 
                                            const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_trace);
    }

    inline std::string filename_element::visit(format &f, 
                                               const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_filename);
    }

    inline std::string line_element::visit(format &f, 
                                           const log_param_t &log_param)
    {
      return f.accept(*this, log_param.m_line);
    }

    inline std::string function_name_element::visit
    (
     format &f, 
     const log_param_t &log_param
    )
    {
      return f.accept(*this, log_param.m_func_name);
    }

    inline std::string pretty_function_name_element::visit
    (
     format &f, 
     const log_param_t &log_param
    )
    {
      return f.accept(*this, log_param.m_func_sig);
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

inline boost::logging::element_list_t operator>>(
  boost::logging::element_list_t lhs, 
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

inline boost::logging::element_list_t operator>>(
  boost::logging::element_list_t lhs, 
  const std::string &s)
{ 
  boost::shared_ptr<boost::logging::literal_element> 
    p(new boost::logging::literal_element(s));
  lhs.push_back(p);
  return lhs;
}

inline
void boost::logging::logger::unformatted_trace(const log_param_t &log_param)
{
#if defined(BOOST_HAS_THREADS)
  boost::mutex::scoped_lock scoped_lock(m_mutex);
#endif // BOOST_HAS_THREADS
  sink_format_assoc_list_t::iterator 
    s_it = m_sink_format_assoc.begin();
  for (; s_it != m_sink_format_assoc.end(); ++s_it)
    {
      boost::logging::format f(boost::logging::trace);
      get<SINK>(*s_it).consume_trace(f, log_param);
    }
}

#endif  // !BOOST_LOGGING_HPP
