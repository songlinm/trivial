#pragma once

#include <map>
#include <limits>

template<class key_type, class value_type>
class span_map 
{

public:
    // full range of key_type with val : K_min -> val
    explicit span_map(value_type const& val) 
    {
        map_.insert(map_.begin(),std::make_pair(std::numeric_limits<key_type>::lowest(),val));
    };

    // Span [keyBegin, keyEnd) mapped to val 
    // Overwrite previous values witin but not values outside
    void assign(key_type const& keyBegin, key_type const& keyEnd, const value_type& val ) 
    {
        if (!(keyBegin < keyEnd))
            return;

        const auto lowerItr = map_.lower_bound(keyBegin); // first no less than keyBegin
        const auto higherItr = map_.upper_bound(keyEnd); // first greater than keyEnd

        if (lowerItr != map_.end())
        {
            // First check if we are pointing at the very front node (smallest key)
            const auto lowerItrIsFront = (lowerItr == map_.begin());
            const auto lowerPreVal = lowerItrIsFront ? 
                lowerItr->second : std::prev(lowerItr)->second;

            // Preserve the high end value as it is before the insert. We need it later
            // to recover the zone on the right end.
            // Essentially highValIns == operator[](keyEnd);
            const auto highValIns = (higherItr == map_.begin()) ? 
                higherItr->second : std::prev(higherItr)->second;

            const auto lowHintItr = map_.erase(lowerItr, higherItr);

            // If the previous key has the same value, skip inserting to avoid 
            // non-canonical value pairs. An exception is if we are already pointing 
            // at the front -- the front is erased above, so need to re-insert
            if (!(lowerPreVal == val) || lowerItrIsFront)
            {
                map_.insert(lowHintItr, std::make_pair(
                    lowerItrIsFront?  std::numeric_limits<key_type>::lowest() : keyBegin, 
                    val));
            }

            if (!(highValIns == val) || (lowerItrIsFront && higherItr == map_.end()))
                map_.insert(higherItr, std::make_pair(keyEnd, highValIns));
        }
        else
        {
            // preserve the current upper value
            const auto highValIns = map_.rbegin()->second;

            if (!(val == highValIns))
            {
                // insert both to end
                map_.insert(lowerItr, std::make_pair(keyBegin, val));
                map_.insert(lowerItr, std::make_pair(keyEnd, highValIns));
            }

        }
    }

    // look-up of the value associated with key
    value_type const& operator[]( key_type const& key ) const 
    {
        return ( --map_.upper_bound(key) )->second;
    }

    auto size() const {return map_.size();}

private:
    std::map<key_type,value_type> map_;
};

