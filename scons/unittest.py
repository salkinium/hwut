#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# unittest.py
#
# Copyright (c) 2009-2015, Roboterclub Aachen e.V.
#
# This file is part of hwut.
#
# hwut is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# hwut is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with hwut.  If not, see <http://www.gnu.org/licenses/>.
#/

from SCons.Script import *

import os
import re
import string
import datetime

# -----------------------------------------------------------------------------
class FunctionScanner:
	# FIXME correct handling of strings and comments
	commentFilter = re.compile(r"/\*.*?\*/", re.DOTALL)

	# find any function 'void test*();'
	functionFilter = re.compile(r"void\s+(test[A-Z]\w*)\s*\([\svoid]*\)\s*;")

	def __init__(self, filename):
		self.text = open(filename).read()

	def getFunctions(self):
		self._stripCommentsAndStrings()
		functions = self.functionFilter.findall(self.text)
		return functions

	def _stripCommentsAndStrings(self):
		self._prepareString()
		self._stripComments()

		lines = self.text.splitlines()
		for i, line in enumerate(lines):
			lines[i] = self._stripCppComments(line)

		self.text = '\n'.join(lines)

	def _prepareString(self):
		self.text = self.text.replace('\r\n', '\n')
		self.text = self.text.replace('\\\n', '')

	def _stripCppComments(self, line):
		index = line.find(r"//")
		if index >= 0:
			return line[:index]
		return line

	def _stripComments(self):
		self.text = self.commentFilter.sub('', self.text)

# -----------------------------------------------------------------------------
def generateClassName(s):
	words = s.split('_')
	name = []
	for word in words:
		index = 0
		for c in word:
			if c.isalpha():
				break
			index += 1

		name.append(word[0:index])
		name.append(word[index].upper())
		name.append(word[index+1:])

	return ''.join(name)

# -----------------------------------------------------------------------------

def unittest_action(target, source, env):
	if not env.has_key('template'):
		raise SCons.Errors.UserError, "Use 'UnittestRunner(..., template = ...)'"
	try:
		import jinja2
	except ImportError:
		env.Error("To use this functionality you need to install the jinja2 template engine")
		Exit(1)

	(template_path, template) = os.path.split(os.path.abspath(env['template']))

	tests = {}
	for file in source:
		# io_stream_test.hpp -> io_stream_test
		basename = os.path.splitext(file.name)[0]

		# io_stream_test -> IoStreamTest
		class_name = generateClassName(basename)
		scanner = FunctionScanner(file.abspath)

		tests[class_name] = {
			'include_path': file.abspath,
			'functions': scanner.getFunctions(),
			'test_name': basename,
		}

	includes = []
	name_strings = []
	tests_cases = []
	for class_name, attr in tests.iteritems():
		includes.append('#include "%s"' % attr['include_path'])

		instance_name = class_name[0].lower() + class_name[1:]
		test_name_string = instance_name + 'Name'
		name_strings.append('FLASH_STORAGE_STRING(%s) = "%s";' % (test_name_string, attr['test_name']))

		str = """\
	hwut::Controller::instance().nextTestSuite(xpcc::accessor::asFlash(%s));
	{
		%s %s;
		""" % (test_name_string, class_name, instance_name)
		
		for function_name in attr['functions']:
			str += """
		%(instance)s.setUp();
		%(instance)s.%(function)s();
		%(instance)s.tearDown();
	""" % { 'instance': instance_name, 'function': function_name }

		str += "}"

		tests_cases.append(str)

	substitutions = {
		'includes': '\n'.join(includes),
		'names': 'namespace\n{\n\t%s\n}' % '\n\t'.join(name_strings),
		'tests': '\n'.join(tests_cases),
	}


	jinja = jinja2.Environment(loader=jinja2.FileSystemLoader(template_path))
	# Jinja2 Line Statements
	jinja.line_statement_prefix = '%%'
	jinja.line_comment_prefix = '%#'
	output = jinja.get_template(template).render(substitutions).encode('utf-8')
	open(target[0].abspath, 'w').write(output)

	return 0

def unittest_emitter(target, source, env):
	if not env.has_key('template'):
		raise SCons.Errors.UserError, "Use 'UnittestRunner(..., template = ...)'"
	Depends(target, env['template'])
	try:
		Depends(target, SCons.Node.Python.Value(env['ARCHITECTURE']))
	except KeyError:
		pass
	header = []
	for file in source:
		if file.name.endswith('_test.hpp'):
			header.append(file)
	return target, header

# -----------------------------------------------------------------------------
def generate(env,**kw):
	env.Append(BUILDERS = {
		'UnittestRunner': Builder(
				action = SCons.Action.Action(unittest_action, "Generate runner file: $TARGET"),
				suffix = '.cpp',
				emitter = unittest_emitter,
				target_factory = env.fs.File)
	})

def exists(env):
	return True
