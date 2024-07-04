/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:51:17 by akernot           #+#    #+#             */
/*   Updated: 2023/06/06 19:00:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/******************************************************************************/
/*                                                                            */
/*              ██████  ██████  ██ ███    ██ ████████ ███████                 */
/*              ██   ██ ██   ██ ██ ████   ██    ██    ██                      */
/*              ██████  ██████  ██ ██ ██  ██    ██    █████                   */
/*              ██      ██   ██ ██ ██  ██ ██    ██    ██                      */
/*              ██      ██   ██ ██ ██   ████    ██    ██                      */
/*                                                                            */
/*                                                                            */
/*  The character % is followed by zero or more of the following              */
/*    flags:                                                                  */
/*                                                                            */
/*       #    The value should be converted to an "alternate form".  For      */
/*            o conversions, the first character of the output string is      */
/*            made zero (by prefixing a 0 if it was not zero already).        */
/*            For x and X conversions, a nonzero result has the string        */
/*            "0x" (or "0X" for X conversions) prepended to it.  For other    */
/*            conversions,  the result is undefined.                          */
/*                                                                            */
/*       0    The value should be zero padded. For d, i, u, x, X, the         */
/*            converted value is padded on the left with zeros rather         */
/*			  than blanks.  If the 0 and - flags both appear, the 0 flag      */
/*			  is ignored. If a precision is given with a numeric conversion   */
/*			  (d, i,  u, x, and X), the 0 flag is ignored.  For other         */
/*			  conversions, the behaviour is undefined.                        */
/*                                                                            */
/*       -    The converted value is to be left adjusted on the field         */
/*            boundary.  (The default is right justification.)  The           */
/*            converted value is padded on the right with blanks, rather      */
/*            than on the left with blanks or zeros.  A - overrides a 0       */
/*            if both are given.                                              */
/*                                                                            */
/*       ' '  (a space) A blank should be left before a positive number       */
/*            (or empty string) produced by a signed conversion.              */
/*                                                                            */
/*       +    A sign (+ or -) should always be placed before a number         */
/*            produced by a signed conversion.  By default, a sign is         */
/*            used only for negative numbers.  A + overrides a space if       */
/*            both are used.                                                  */
/*                                                                            */
/*    Field width                                                             */
/*       An optional decimal digit string (with non zero first digit)         */
/*       specifying a minimum field width.  If the converted value has        */
/*       fewer characters than the field width, it will be padded with        */
/*       spaces on the left (or right, if the left-adjustment flag has        */
/*       been given).  A negative field width is taken as a '-' flag          */
/*       followed by a positive field width.  In no case does a               */
/*       nonexistent or small field width cause truncation of a field;        */
/*       if the result of a conversion is wider than the field width,         */
/*       the field is expanded to contain the conversion result.              */
/*                                                                            */
/*    Precision                                                               */
/*       An optional precision, in the form of a period ('.')  followed by    */
/*       an optional decimal digit string. If the precision is given as just  */
/*       '.', the precision is taken to be zero.  A negative precision is     */
/*       taken as if the precision wereomitted.  This gives the minimum       */
/*       number of digits to appear for d, i, o, x, and X conversions, or     */
/*       the maximum number of characters to be printed from a string for     */
/*       s conversions.                                                       */
/*                                                                            */
/*       d, i   The int argument is converted to signed decimal notation.     */
/*              The precision, if any, gives the minimum number of digits     */
/*              that must appear; if the converted value requires fewer       */
/*              digits, it is padded on the left with zeros.  The default     */
/*              precision is 1.  When 0 is printed with an explicit           */
/*              precision 0, the output is empty.                             */
/*                                                                            */
/*       u, x, X    The unsigned int argument is converted to, or unsigned    */
/*              hexadecimal (x and X) notation.  The letters abcdef are used  */
/*              for x conversions; the letters ABCDEF are used for X          */
/*              conversions.  The precision, if any, gives the minimum number */
/*              of digits that must appear; if the converted value requires   */
/*              fewer digits, it is padded on the left with zeros. The        */
/*              default precision is 1.  When 0 is printed with an explicit   */
/*              precision 0, the output is empty.                             */
/*                                                                            */
/*       c      If no l modifier is present, the int argument is converted    */
/*              to an unsigned char, and the resulting character is           */
/*              written.  If an l modifier is present, the wint_t (wide       */
/*              character) argument is converted to a multibyte sequence      */
/*              by a call to the wcrtomb(3) function, with a conversion       */
/*              state starting in the initial state, and the resulting        */
/*              multibyte string is written.                                  */
/*                                                                            */
/*       s      If no l modifier is present: the const char * argument is     */
/*              expected to be a pointer to an array of character type        */
/*              (pointer to a string).  Characters from the array are         */
/*              written up to (but not including) a terminating null byte     */
/*              ('\0'); if a precision is specified, no more than the         */
/*              number specified are written.  If a precision is given, no    */
/*              null byte need be present; if the precision is not            */
/*              specified, or is greater than the size of the array, the      */
/*              array must contain a terminating null byte.                   */
/*                                                                            */
/*       p      The void * pointer argument is printed in hexadecimal (as     */
/*              if by %#x).                                                   */
/*                                                                            */
/*       %      A '%' is written.  No argument is converted.  The complete    */
/*              conversion specification is '%%'.                             */
/*                                                                            */
/******************************************************************************/

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_switches
{
	int	space_or_plus;
	int	pad;
	int	keep_zeros;
	int	precision;
	int	min_width;
}	t_switches;

int	ft_printf(const char *format, ...);
#endif
