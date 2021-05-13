#
# Emulative hypothesis for top-down/approach_2/README.md
# Use python 2.x to run it.
#
# Paulus Gandung Prakosa (rvn.plvhx@gmail.com)
#

import sys

def get_func_name():
	return sys._getframe().f_back.f_code.co_name

class LexerValue(object):
	def __init__(self, buf, token_type):
		self.buf = buf
		self.token_type = token_type

	def get_value(self):
		return self.buf

	def get_token_type(self):
		return self.token_type

class Lexer(object):
	def __init__(self, buf):
		self.buf = buf
		self.cursor = 0
		self.buflen = len(self.buf)
		self.tokens = []

	def get_cursor(self):
		return self.cursor

	def get_buffer(self):
		return self.buf

	def get_buffer_at(self, i):
		return self.buf[i]

	def get_buffer_len(self):
		return self.buflen

	def get_current(self):
		return self.get_buffer_at(self.get_cursor())

	def __inc_cursor__(self):
		self.cursor = self.cursor + 1

	def __dec_cursor__(self):
		self.cursor = self.cursor - 1

	def advance(self):
		if self.get_cursor() >= self.get_buffer_len():
			return False

		self.__inc_cursor__()

	def step_back(self):
		if self.get_cursor() == 0:
			return False

		self.__dec_cursor__()

	def __handle_open_parenthesis__(self):
		self.tokens.append(LexerValue('(', 'OPEN_PARENTHESIS'))

	def __handle_close_parenthesis__(self):
		self.tokens.append(LexerValue(')', 'CLOSE_PARENTHESIS'))

	def __handle_plus_operator__(self):
		self.tokens.append(LexerValue('+', 'PLUS_OP'))

	def __is_lower_alpha__(self, c):
		if c[0] >= 'a' and c[0] <= 'z':
			return True
		return False

	def __is_upper_alpha__(self, c):
		if c[0] >= 'A' and c[0] <= 'Z':
			return True
		return False

	def __is_digit__(self, c):
		if c[0] >= '0' and c[0] <= '9':
			return True
		return False

	def __is_underscore__(self, c):
		if c[0] == '_':
			return True
		return False

	def __handle_variable__(self):
		buf = ''
		token = self.get_current()

		'''
		Variable names begin with underscore, lower alpha,
		or upper alpha.
		'''
		if not self.__is_underscore__(token) and \
		   not self.__is_lower_alpha__(token) and \
		   not self.__is_upper_alpha__(token):
		   return False

		buf += token

		self.advance()

		while self.get_cursor() < self.get_buffer_len():
			token = self.get_current()

			if not self.__is_underscore__(token) and \
			   not self.__is_lower_alpha__(token) and \
			   not self.__is_upper_alpha__(token) and \
			   not self.__is_digit__(token):
			   self.step_back()
			   break

			buf += token
			self.advance()

		self.tokens.append(LexerValue(buf, 'VARIABLE'))

	def lex(self):
		while self.get_cursor() < self.get_buffer_len():
			if self.get_current().isspace():
				self.advance()
				continue

			if self.get_current() == '(':
				self.__handle_open_parenthesis__()
			elif self.get_current() == ')':
				self.__handle_close_parenthesis__()
			elif self.get_current() == '+':
				self.__handle_plus_operator__()
			else:
				self.__handle_variable__()

			# advance buffer cursor position.
			self.advance()

	def get_tokens(self):
		return self.tokens

	def __repr__(self):
		buf = ''
		for el in self.get_tokens():
			buf += "(val: {}, kind: {})\n".format(el.get_value(), el.get_token_type())
		return buf

class Ast(object):
	def __init__(self, val, kind):
		self.val = val
		self.kind = kind
		self.parent = None
		self.num_childs = 0
		self.num_spaces = 0
		self.childs = []

	def add_child(self, child):
		self.childs.append(child)
		self.num_childs = self.num_childs + 1

	def set_parent(self, parent):
		self.parent = parent

	def get_val(self):
		return self.val

	def get_kind(self):
		return self.kind

class Parser(object):
	def __init__(self, tokens):
		self.tokens = tokens
		self.tlen = len(self.tokens)
		self.stack = []
		self.cursor = 0
		self.ast = None
		self.is_error = False
		self.error_msg = ''
		self.dumped_ast = ''

	def get_cursor(self):
		return self.cursor

	def advance(self):
		if self.cursor >= self.get_token_len():
			self.set_is_error(True)
			return

		self.cursor = self.cursor + 1

	def step_back(self):
		if self.cursor == 0:
			return False
		self.cursor = self.cursor - 1

	def get_token_len(self):
		return self.tlen

	def current(self):
		if self.get_cursor() >= self.get_token_len():
			return False
		return self.tokens[self.get_cursor()]

	def peek(self):
		if self.get_cursor() + 1 >= self.get_token_len():
			return False
		return self.tokens[self.get_cursor() + 1]

	def look_behind(self):
		if self.get_cursor() - 1 < 0:
			return False
		return self.tokens[self.get_cursor() - 1]

	def get_is_error(self):
		return self.is_error

	def set_is_error(self, val):
		self.is_error = val

	def get_error_msg(self):
		return self.error_msg

	def set_error_msg(self, val):
		self.error_msg = val

	def __expression__(self):
		if self.get_is_error():
			return

		self.__addition__()

	def __grouped__(self):
		if self.get_is_error():
			return

		if self.current() != False and \
		   self.current().get_token_type() == 'OPEN_PARENTHESIS':
			self.stack.append(self.current().get_value())
			self.advance()

			while self.current() != False and \
			      self.current().get_token_type() != 'CLOSE_PARENTHESIS':
				self.__expression__()

		if self.current() != False and \
		   self.current().get_token_type() == 'CLOSE_PARENTHESIS':
			if len(self.stack) == 0:
				self.set_is_error(True)
				self.set_error_msg("Missing opening parenthesis.")
				return

			# ignore the popped data.
			self.stack.pop()
			self.advance()

	def __variable__(self):
		if self.get_is_error():
			return

		if not self.current():
			self.set_is_error(True)
			return

		self.ast = Ast(self.current().get_value(), 'VARIABLE')
		self.advance()

	def __addition__(self):
		if self.get_is_error():
			return

		if not self.current():
			return

		if self.current().get_token_type() == 'OPEN_PARENTHESIS' or \
		   self.current().get_token_type() == 'CLOSE_PARENTHESIS':
			self.__grouped__()
			return

		if self.current().get_token_type() == 'PLUS_OP':
			if self.ast == None:
				self.set_is_error(True)
				self.set_error_msg("'+' sign must be preceeded by variable operand or expression.")
				return

			self.advance()

			if self.get_is_error():
				self.set_error_msg("Invalid expression. '+' must be succeeded by variable or expression. It looks like lexer token stream is reaching EOF.")
				return

			prev = self.ast

			self.__expression__()

			if self.ast == prev or \
			   (self.ast.kind != 'VARIABLE' and self.ast.kind != 'EXPR'):
				self.set_is_error(True)
				self.set_error_msg("'+' sign must be succeeded by variable operand or expression.")
				return

			ast = Ast(None, 'EXPR')
			ast.add_child(prev)
			ast.add_child(Ast('+', 'ADDITION'))
			ast.add_child(self.ast)
			ast.add_child(None)

			# process child[0].
			if ast.childs[0].kind == 'EXPR':
				self.__align_ast_spacing__(ast.childs[0], ast)
			elif ast.childs[0].kind == 'VARIABLE':
				ast.childs[0].num_spaces = ast.num_spaces + 1
				ast.childs[0].parent = ast

			# process child[1].
			ast.childs[1].num_spaces = ast.num_spaces + 1
			ast.childs[1].parent = ast

			# process child[2].
			if ast.childs[2].kind == 'EXPR':
				self.__align_ast_spacing__(ast.childs[2], ast)
			elif ast.childs[2].kind == 'VARIABLE':
				ast.childs[2].num_spaces = ast.num_spaces + 1
				ast.childs[2].parent = ast

			self.ast = ast
			return

		if self.current().get_token_type() == 'VARIABLE':
			self.__variable__()

	def parse(self):
		while True:
			if self.get_cursor() >= self.get_token_len() or \
			   self.get_is_error():
				break
			self.__expression__()

		# check for underlying opening
		# parenthesis on the stack.
		if len(self.stack) > 0:
			self.set_is_error(True)
			self.set_error_msg("Missing closing parenthesis.")

	def get_ast(self):
		return self.ast

	def __align_ast_spacing__(self, ast, parent):
		if ast.kind != 'EXPR':
			return

		ast.num_spaces = parent.num_spaces + 1
		ast.parent = parent

		for el in ast.childs:
			if el == None:
				break

			if el.kind == 'EXPR':
				self.__align_ast_spacing__(el, ast)

			el.num_spaces = ast.num_spaces + 1
			el.parent = ast

	def __dump_ast__(self, ast):
		if self.get_is_error():
			self.dumped_ast = self.get_error_msg()
			return

		if ast == None:
			return

		self.dumped_ast += "{}(type: ({}), value: ({}))\n".format(
			'  '*(ast.num_spaces),
			ast.get_kind(),
			ast.get_val()
		)

		for el in ast.childs:
			self.__dump_ast__(el)

	def __repr__(self):
		self.__dump_ast__(self.get_ast())
		return self.dumped_ast.rstrip(chr(0x0a))

def runner(buf):
	print 'syntax: {}'.format(buf)
	print '{} generated AST {}'.format('-'*(5), '-'*(5))
	lexer = Lexer(buf)
	lexer.lex()
	parser = Parser(lexer.get_tokens())
	parser.parse()
	print repr(parser)
	print '-'*(25) + chr(0x0a)

possibility = '''
a + b
(a) + b
((a)) + b
a + (b)
a + ((b))
(a) + (b)
((a)) + ((b))
'''

if __name__ == '__main__':
	for el in possibility.rstrip(chr(0x0a)).lstrip(chr(0x0a)).split(chr(0x0a)):
		runner(el)
