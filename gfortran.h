#include <stdint.h>
#include <stddef.h>



#define CHARACTER2(name) \
              int32_t name ## _len; \
              char * name

#define CHARACTER1(name) \
              char * name; \
              int32_t name ## _len
#define GFC_INTEGER_4 int32_t
typedef GFC_INTEGER_4 GFC_IO_INT;

typedef struct st_parameter_common
{
  GFC_INTEGER_4 flags;
  GFC_INTEGER_4 unit;
  const char *filename;
  GFC_INTEGER_4 line;
  CHARACTER2 (iomsg);
  GFC_INTEGER_4 *iostat;
}
st_parameter_common;


typedef struct
{
  st_parameter_common common;
  int recl_in;
  int a;
  int b;
  int c;
  CHARACTER2 (file);
  CHARACTER1 (status);
  CHARACTER2 (access);
  CHARACTER1 (form);
  CHARACTER2 (blank);
  CHARACTER1 (position);
  CHARACTER2 (action);
  CHARACTER1 (delim);
  CHARACTER2 (pad);
  CHARACTER1 (convert);
  CHARACTER2 (decimal);
  CHARACTER1 (encoding);
  CHARACTER2 (round);
  CHARACTER1 (sign);
  CHARACTER2 (asynchronous);
  GFC_INTEGER_4 *newunit;
  GFC_INTEGER_4 readonly;
  CHARACTER2 (cc);
  CHARACTER1 (share);
}
st_parameter_open;


typedef ptrdiff_t index_type;
typedef struct descriptor_dimension
{
  index_type _stride;
  index_type lower_bound;
  index_type _ubound;
}
descriptor_dimension;

typedef struct dtype_type
{
  size_t elem_len;
  int version;
  signed char rank;
  signed char type;
  signed short attribute;
}
dtype_type;

#define GFC_ARRAY_DESCRIPTOR(type) \
struct {\
  type *base_addr;\
  size_t offset;\
  dtype_type dtype;\
  index_type span;\
  descriptor_dimension dim[];\
}
typedef GFC_ARRAY_DESCRIPTOR (char) gfc_array_char;

typedef enum
{READING, WRITING, LIST_READING, LIST_WRITING}
unit_mode;

typedef enum
{ BLANK_NULL, BLANK_ZERO, BLANK_UNSPECIFIED }
unit_blank;

typedef enum
{ SIGN_PROCDEFINED, SIGN_SUPPRESS, SIGN_PLUS, SIGN_UNSPECIFIED }
unit_sign;

typedef enum
{ ADVANCE_YES, ADVANCE_NO, ADVANCE_UNSPECIFIED }
unit_advance;

typedef enum
{ BT_UNKNOWN = 0, BT_INTEGER, BT_LOGICAL, BT_REAL, BT_COMPLEX,
  BT_DERIVED, BT_CHARACTER, BT_CLASS, BT_PROCEDURE, BT_HOLLERITH, BT_VOID,
  BT_ASSUMED, BT_UNION
}
bt;

typedef struct array_loop_spec
{
  /* Index counter for this dimension.  */
  index_type idx;

  /* Start for the index counter.  */
  index_type start;

  /* End for the index counter.  */
  index_type end;

  /* Step for the index counter.  */
  index_type step;
}
array_loop_spec;

typedef struct namelist_type
{
  /* Object type.  */
  bt type;

  /* Object name.  */
  char * var_name;

  /* Address for the start of the object's data.  */
  void * mem_pos;

  /* Address of specific DTIO subroutine.  */
  void * dtio_sub;

  /* Address of vtable if dtio_sub non-null.  */
  void * vtable;

  /* Flag to show that a read is to be attempted for this node.  */
  int touched;

  /* Length of intrinsic type in bytes.  */
  int len;

  /* Rank of the object.  */
  int var_rank;

  /* Overall size of the object in bytes.  */
  index_type size;

  /* Length of character string.  */
  index_type string_length;

  descriptor_dimension * dim;
  array_loop_spec * ls;
  struct namelist_type * next;
}
namelist_info;

// omit a bit
typedef void (*formatted_dtio)(void *, GFC_INTEGER_4 *, char *);
typedef void (*unformatted_dtio)(void *, GFC_INTEGER_4 *, GFC_INTEGER_4 *, char *);

typedef struct st_parameter_dt
{
  st_parameter_common common;
  GFC_IO_INT rec;
  GFC_IO_INT *size, *iolength;
  gfc_array_char *internal_unit_desc;
  CHARACTER1 (format);
  CHARACTER2 (advance);
  CHARACTER1 (internal_unit);
  CHARACTER2 (namelist_name);
  GFC_INTEGER_4 *id;
  GFC_IO_INT pos;
  CHARACTER1 (asynchronous);
  CHARACTER2 (blank);
  CHARACTER1 (decimal);
  CHARACTER2 (delim);
  CHARACTER1 (pad);
  CHARACTER2 (round);
  CHARACTER1 (sign);
  /* Private part of the structure.  The compiler just needs
     to reserve enough space.  */
  union
    {
      struct
	{
	  void (*transfer) (struct st_parameter_dt *, bt, void *, int,
			    size_t, size_t);
	  struct gfc_unit *current_unit;
	  /* Item number in a formatted data transfer.  Also used in namelist
	     read_logical as an index into line_buffer.  */
	  int item_count;
	  unit_mode mode;
	  unit_blank blank_status;
	  unit_sign sign_status;
	  int scale_factor;
	  /* Maximum righthand column written to.  */
	  int max_pos;
	  /* Number of skips + spaces to be done for T and X-editing.  */
	  int skips;
	  /* Number of spaces to be done for T and X-editing.  */
	  int pending_spaces;
	  /* Whether an EOR condition was encountered. Value is:
	       0 if no EOR was encountered
	       1 if an EOR was encountered due to a 1-byte marker (LF)
	       2 if an EOR was encountered due to a 2-bytes marker (CRLF) */
	  int sf_seen_eor;
	  unit_advance advance_status;
	  unsigned reversion_flag : 1; /* Format reversion has occurred.  */
	  unsigned first_item : 1;
	  unsigned seen_dollar : 1;
	  unsigned eor_condition : 1;
	  unsigned no_leading_blank : 1;
	  unsigned char_flag : 1;
	  unsigned input_complete : 1;
	  unsigned at_eol : 1;
	  unsigned comma_flag : 1;
	  /* A namelist specific flag used in the list directed library
	     to flag that calls are being made from namelist read (e.g. to
	     ignore comments or to treat '/' as a terminator)  */
	  unsigned namelist_mode : 1;
	  /* A namelist specific flag used in the list directed library
	     to flag read errors and return, so that an attempt can be
	     made to read a new object name.  */
	  unsigned nml_read_error : 1;
	  /* A sequential formatted read specific flag used to signal that a
	     character string is being read so don't use commas to shorten a
	     formatted field width.  */
	  unsigned sf_read_comma : 1;
	  /* A namelist specific flag used to enable reading input from 
	     line_buffer for logical reads.  */
	  unsigned line_buffer_enabled : 1;
	  /* An internal unit specific flag used to identify that the associated
	     unit is internal.  */
	  unsigned unit_is_internal : 1;
	  /* An internal unit specific flag to signify an EOF condition for list
	     directed read.  */
	  unsigned at_eof : 1;
	  /* Used for g0 floating point output.  */
	  unsigned g0_no_blanks : 1;
	  /* Used to signal use of free_format_data.  */
	  unsigned format_not_saved : 1;
	  /* A flag used to identify when a non-standard expanded namelist read
	     has occurred.  */
	  unsigned expanded_read : 1;
	  /* Flag to indicate if the statement has async="YES". */
	  unsigned async : 1;
	  /* 12 unused bits.  */

	  int child_saved_iostat;
	  int nml_delim;
	  int repeat_count;
	  int saved_length;
	  int saved_used;
	  bt saved_type;
	  char *saved_string;
	  char *scratch;
	  char *line_buffer;
	  struct format_data *fmt;
	  namelist_info *ionml;
#ifdef HAVE_NEWLOCALE
	  locale_t old_locale;
#endif
	  /* Current position within the look-ahead line buffer.  */
	  int line_buffer_pos;
	  /* Storage area for values except for strings.  Must be
	     large enough to hold a complex value (two reals) of the
	     largest kind.  */
	  char value[32];
	  GFC_IO_INT not_used; /* Needed for alignment. */
	  formatted_dtio fdtio_ptr;
	  unformatted_dtio ufdtio_ptr;
	  /* With CC_FORTRAN, the first character of a record determines the
	     style of record end (and start) to use. We must mark down the type
	     when we write first in write_a so we remember the end type later in
	     next_record_w.  */
	  struct
	    {
	      unsigned type : 6; /* See enum cc_fortran.  */
	      unsigned len  : 2; /* Always 0, 1, or 2.  */
	      /* The union is updated after start-of-record is written.  */
	      union
		{
		  char start; /* Output character for start of record.  */
		  char end;   /* Output character for end of record.  */
		} u;
	    } cc;
	} p;
      /* This pad size must be equal to the pad_size declared in
	 trans-io.c (gfc_build_io_library_fndecls).  The above structure
	 must be smaller or equal to this array.  */
      char pad[16 * sizeof (char *) + 32 * sizeof (int)];
    } u;
}
st_parameter_dt;