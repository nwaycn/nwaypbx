/**
 * Read the configuration file
 *
 * @copyright           (C) 2014  widuu
 * @lastmodify          2014-2-22
 * @website		http://www.widuu.com
 *
 */

package config

import (
	"bufio"
	"fmt"
	"github.com/nwaycn/NwayPBX-go/gw_web/controllers/log"
	"io"
	"os"
	"strconv"
	"strings"
)

type Config struct {
	filepath string                         //your ini file path directory+file
	conflist []map[string]map[string]string //configuration information slice
}

//Create an empty configuration file
func SetConfig(filepath string) (*Config, bool) {
	c := new(Config)
	c.filepath = filepath
	file, err := os.Open(c.filepath)
	defer file.Close()
	if err != nil {
		CheckErr(err)
		logger.Error("load config file err:", err)
		return nil, false
	}
	return c, true
}

//To obtain corresponding value of the key values
func (c *Config) GetValue(section, name string) (string, bool) {
	//c.ReadList()

	conf := c.ReadList()
	for _, v := range conf {
		for key, value := range v {
			if key == section {
				return value[name], true
			}
		}
	}
	return "no value", false
}

//to return a int value of the key value
//param d: a default value
//param section: the section name
//param name: the param name
func (c *Config) GetInt(section, name string, d int) int {
	svalue, bvalue := c.GetValue(section, name)
	if bvalue {
		if rvalue, err := strconv.Atoi(svalue); err == nil {
			return rvalue
		} else {
			logger.Error("convert the value failed from section:%s,name:%s ' value", section, name)
			return d
		}
	} else {
		logger.Error("not found the section:%q,name:%q ' value", section, name)
		return d
	}
}

//Set the corresponding value of the key value, if not add, if there is a key change
func (c *Config) SetValue(section, key, value string) bool {
	c.ReadList()
	data := c.conflist
	var ok bool
	var index = make(map[int]bool)
	var conf = make(map[string]map[string]string)
	for i, v := range data {
		_, ok = v[section]
		index[i] = ok
	}

	i, ok := func(m map[int]bool) (i int, v bool) {
		for i, v := range m {
			if v == true {
				return i, true
			}
		}
		return 0, false
	}(index)

	if ok {
		c.conflist[i][section][key] = value
		return true
	} else {
		conf[section] = make(map[string]string)
		conf[section][key] = value
		c.conflist = append(c.conflist, conf)
		return true
	}

	return false
}

//Delete the corresponding key values
func (c *Config) DeleteValue(section, name string) bool {
	c.ReadList()
	data := c.conflist
	for i, v := range data {
		for key, _ := range v {
			if key == section {
				delete(c.conflist[i][key], name)
				return true
			}
		}
	}
	return false
}

//List all the configuration file
func (c *Config) ReadList() []map[string]map[string]string {

	file, err := os.Open(c.filepath)
	if err != nil {
		CheckErr(err)
	}
	defer file.Close()
	var data map[string]map[string]string
	var section string
	//var sec map[string]string
	buf := bufio.NewReader(file)
	for {
		l, err := buf.ReadString('\n')
		line := strings.TrimSpace(l)
		fmt.Println(line)
		if err != nil {
			if err != io.EOF {
				CheckErr(err)
			}
			if len(line) == 0 {
				break
			}
		}
		if strings.IndexAny(line, "[") > 0 && strings.IndexAny(line, "]") > 0 && strings.IndexAny(line, "]") > strings.IndexAny(line, "[") {
			fmt.Println("cut the line")
			line = line[strings.IndexAny(line, "[") : strings.IndexAny(line, "]")+1]
			fmt.Println(line)
		}
		switch {
		case len(line) == 0:
		case line[0] == '[' && line[len(line)-1] == ']':
			section = strings.TrimSpace(line[1 : len(line)-1])
			data = make(map[string]map[string]string, 10)
			data[section] = make(map[string]string, 10)
			//sec = make(map[string]string, 0)
		default:
			i := strings.IndexAny(line, "=")

			fmt.Println(i)
			if i > 1 {
				value := strings.TrimSpace(line[i+1 : len(line)])
				myfield := line[0:i]
				fmt.Println(section, myfield, ":", value)
				data[section][myfield] = value
				//data[section][strings.TrimSpace(line[0:i])] = value
				//data = append(data[section][strings.TrimSpace(line[0:i])],value)
				//data[section] = append(data[section],strings.TrimSpace(line[0:i]) , value)
				if c.uniquappend(section) == true {
					c.conflist = append(c.conflist, data)

				}
			}
		}

	}

	return c.conflist
}

func CheckErr(err error) string {
	if err != nil {
		return fmt.Sprintf("Error is :'%s'", err.Error())
	}
	return "Notfound this error"
}

//Ban repeated appended to the slice method
func (c *Config) uniquappend(conf string) bool {
	for _, v := range c.conflist {
		for k, _ := range v {
			if k == conf {
				return false
			}
		}
	}
	return true
}
